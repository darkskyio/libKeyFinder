/*************************************************************************
 
 Copyright 2011-2013 Ibrahim Sha'ath
 
 This file is part of KeyFinder.
 
 KeyFinder is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 KeyFinder is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with KeyFinder.  If not, see <http://www.gnu.org/licenses/>.
 
 *************************************************************************/

#include "decoderlibav.h"

//QMutex codecMutex; // I don't think this should be necessary if I get the lock manager right.

KeyFinder::AudioData LibAvDecoder::decodeFile(std::string filePath, const int maxDuration){
	
	//QMutexLocker codecMutexLocker(&codecMutex); // mutex the preparatory section of this method
	av_register_all();
	
	AVCodec *codec = NULL;
	AVFormatContext *fCtx = NULL;
	AVCodecContext *cCtx = NULL;
	AVDictionary* dict = NULL;
	
	// convert filepath
	const char* filePathCh = filePath.c_str();
	
	// open file
	int openInputResult = avformat_open_input(&fCtx, filePathCh, NULL, NULL);
	if(openInputResult != 0){
		printf("Could not open file %s (%d)\n", filePathCh, openInputResult);
		throw KeyFinder::Exception(libavCouldNotOpenFile(openInputResult).c_str());
	}
	
	if(avformat_find_stream_info(fCtx, NULL) < 0){
		avformat_close_input(&fCtx);
		printf("Could not find stream information for file %s\n", filePathCh);
		throw KeyFinder::Exception(libavCouldNotFindStreamInformation().c_str());
	}
	int audioStream = -1;
	for(int i=0; i<(signed)fCtx->nb_streams; i++){
		if(fCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
			audioStream = i;
			break;
		}
	}
	if(audioStream == -1){
		avformat_close_input(&fCtx);
		printf("Could not find an audio stream for file %s\n", filePathCh);
		throw KeyFinder::Exception(libavCouldNotFindAudioStream().c_str());
	}
	
	// Determine duration
	int durationSeconds = (int)fCtx->duration / AV_TIME_BASE;
	int durationMinutes = durationSeconds / 60;
	// First condition is a hack for bizarre overestimation of some MP3s
	if(durationMinutes < 720 && durationSeconds > maxDuration * 60){
		avformat_close_input(&fCtx);
		printf("Duration of file %s (%d:%d) exceeds specified maximum (%d:00)\n", filePathCh, durationMinutes, durationSeconds % 60, maxDuration);
		throw KeyFinder::Exception(durationExceedsPreference(durationMinutes, durationSeconds % 60, maxDuration).c_str());
	}
	
	// Determine stream codec
	cCtx = fCtx->streams[audioStream]->codec;
	codec = avcodec_find_decoder(cCtx->codec_id);
	if(codec == NULL){
		avformat_close_input(&fCtx);
		printf("Audio stream has unsupported codec in file %s\n", filePathCh);
		throw KeyFinder::Exception(libavUnsupportedCodec().c_str());
	}
	
	// Open codec
	int codecOpenResult = avcodec_open2(cCtx, codec, &dict);
	if(codecOpenResult < 0){
		avformat_close_input(&fCtx);
		printf("Could not open audio codec %s (%d) for file %s\n", codec->long_name, codecOpenResult, filePathCh);
		throw KeyFinder::Exception(libavCouldNotOpenCodec(codec->long_name, codecOpenResult).c_str());
	}
	
	ReSampleContext* rsCtx = av_audio_resample_init(
													cCtx->channels, cCtx->channels,
													cCtx->sample_rate, cCtx->sample_rate,
													AV_SAMPLE_FMT_S16, cCtx->sample_fmt,
													0, 0, 0, 0);
	if(rsCtx == NULL){
		avcodec_close(cCtx);
		avformat_close_input(&fCtx);
		printf("Could not create ReSampleContext for file %s\n", filePathCh);
		throw KeyFinder::Exception(libavCouldNotCreateResampleContext().c_str());
	}
	
	//printf("DEBUG: Decoding %s (%s, %d)\n", filePathCh, av_get_sample_fmt_name(cCtx->sample_fmt), cCtx->sample_rate);
	
	//codecMutexLocker.unlock();
	
	// Prep buffer
	KeyFinder::AudioData audio;
	audio.setFrameRate(cCtx->sample_rate);
	audio.setChannels(cCtx->channels);
	// Decode stream
	AVPacket avpkt;
	int badPacketCount = 0;
	int badPacketThreshold = 100;
	while(true){
		av_init_packet(&avpkt);
		if(av_read_frame(fCtx, &avpkt) < 0)
			break;
		if(avpkt.stream_index == audioStream){
			try{
				int result = decodePacket(cCtx, rsCtx, &avpkt, audio);
				if(result != 0){
					if(badPacketCount < badPacketThreshold){
						badPacketCount++;
					}else{
						avcodec_close(cCtx);
						avformat_close_input(&fCtx);
						printf("Too many bad packets (%d) while decoding file %s\n", badPacketCount, filePathCh);
						throw KeyFinder::Exception(libavTooManyBadPackets(badPacketThreshold).c_str());
					}
				}
			}catch(KeyFinder::Exception& e){
				printf("Encountered KeyFinder::Exception (%s) while decoding file %s\n", e.what(), filePathCh);
				throw e;
			}
		}
		av_free_packet(&avpkt);
	}
	
	//codecMutexLocker.relock();
	audio_resample_close(rsCtx);
	int codecCloseResult = avcodec_close(cCtx);
	if(codecCloseResult < 0){
		printf("Error closing audio codec: %s (%d)\n", codec->long_name, codecCloseResult);
	}
	//codecMutexLocker.unlock();
	
	avformat_close_input(&fCtx);
	return audio;
}

LibAvDecoder::LibAvDecoder(){
	frameBufferSize = ((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2) * sizeof(uint8_t);
	frameBuffer = (uint8_t*)av_malloc(frameBufferSize);
	frameBufferConverted = (uint8_t*)av_malloc(frameBufferSize);
}

LibAvDecoder::~LibAvDecoder(){
	av_free(frameBuffer);
	av_free(frameBufferConverted);
}

int LibAvDecoder::decodePacket(AVCodecContext* cCtx, ReSampleContext* rsCtx, AVPacket* originalPacket, KeyFinder::AudioData& audio){
	// copy packet so we can shift data pointer about without endangering garbage collection
	AVPacket tempPacket;
	tempPacket.size = originalPacket->size;
	tempPacket.data = originalPacket->data;
	// loop in case audio packet contains multiple frames
	while(tempPacket.size > 0){
		int dataSize = frameBufferSize;
		int16_t* dataBuffer = (int16_t*)frameBuffer;
		int bytesConsumed = avcodec_decode_audio3(cCtx, dataBuffer, &dataSize, &tempPacket);
		if(bytesConsumed < 0){ // error
			tempPacket.size = 0;
			return 1;
		}
		tempPacket.data += bytesConsumed;
		tempPacket.size -= bytesConsumed;
		if(dataSize <= 0)
			continue; // nothing decoded
		int newSamplesDecoded = dataSize / av_get_bytes_per_sample(cCtx->sample_fmt);
		// Resample if necessary
		if(cCtx->sample_fmt != AV_SAMPLE_FMT_S16){
			int resampleResult = audio_resample(rsCtx, (short*)frameBufferConverted, (short*)frameBuffer, newSamplesDecoded);
			if(resampleResult < 0){
				throw KeyFinder::Exception(libavCouldNotResample().c_str());
			}
			dataBuffer = (int16_t*)frameBufferConverted;
		}
		int oldSampleCount = audio.getSampleCount();
		try{
			audio.addToSampleCount(newSamplesDecoded);
		}catch(KeyFinder::Exception& e){
			throw e;
		}
		for(int i = 0; i < newSamplesDecoded; i++){
			audio.setSample(oldSampleCount+i, (float)dataBuffer[i]);
		}
	}
	return 0;
}
