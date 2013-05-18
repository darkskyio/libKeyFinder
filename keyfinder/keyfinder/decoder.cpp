//
//  decoder.cpp
//  keyfinder
//
//  Created by James Reuss on 16/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#include "decoder.h"

using namespace std;

#pragma mark -
#pragma mark Lifecycle Functions

Decoder::Decoder()
{
	frameBufferSize = ((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2) * sizeof(uint8_t);
	frameBuffer = (uint8_t*)av_malloc(frameBufferSize);
	frameBufferConverted = (uint8_t*)av_malloc(frameBufferSize);
}

Decoder::~Decoder()
{
	av_free(frameBuffer);
	av_free(frameBufferConverted);
}





#pragma mark -
#pragma mark Private Helpers

int Decoder::decodePacket(AVCodecContext *codec, ReSampleContext *resample, AVPacket *originalPacket, KeyFinder::AudioData &audio)
{
	// Copy packet so we can shift data pointer about without endangering garbage collection.
	AVPacket tempPacket;
	tempPacket.size = originalPacket->size;
	tempPacket.data = originalPacket->data;
	
	// Loop in case audio packet contains multiple frames.
	while(tempPacket.size > 0){
		int dataSize = frameBufferSize;
		int16_t* dataBuffer = (int16_t*)frameBuffer;
		
		int bytesConsumed = avcodec_decode_audio3(codec, dataBuffer, &dataSize, &tempPacket);
		if(bytesConsumed < 0){ // error
			tempPacket.size = 0;
			return DECODER_ERROR_PACKET;
		}
		tempPacket.data += bytesConsumed;
		tempPacket.size -= bytesConsumed;
		if(dataSize <= 0)
			continue; // nothing decoded
		int newSamplesDecoded = dataSize / av_get_bytes_per_sample(codec->sample_fmt);
		// Resample if necessary
		if(codec->sample_fmt != AV_SAMPLE_FMT_S16){
			int resampleResult = audio_resample(resample, (short*)frameBufferConverted, (short*)frameBuffer, newSamplesDecoded);
			if(resampleResult < 0){
				throw KeyFinder::Exception(DECODER_ERROR_PACKET_RESAMPLE);
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






#pragma mark -
#pragma mark Main Functions

int Decoder::decodeFile(std::string path, const int maxDuration, KeyFinder::AudioData &audioData)
{
	//--------------------------------------------------------//
	// Decode the audio!
	//--------------------------------------------------------//
	av_register_all();
	
	AVFormatContext *formatCtx = NULL;
	AVCodecContext	*codecCtx = NULL;
	AVCodec *codec = NULL;
	AVDictionary *dict = NULL;
	const char* filepath = path.c_str();
	
	// Open the file.
	int open_input_result = avformat_open_input(&formatCtx, filepath, NULL, NULL);
	if (open_input_result != 0) {
		char *errbuf = (char*)malloc(50 * sizeof(char));
		av_strerror(open_input_result, errbuf, 50);
		cout << "AV Error! " << errbuf << " (" << open_input_result << ").\n";
		return DECODER_ERROR_WITH_AV(open_input_result);
	}
	
	// Get the stream info for the audio.
	if (avformat_find_stream_info(formatCtx, NULL) < 0) {
		avformat_close_input(&formatCtx);
		cout << "AV Error! " << "Could not find the stream information for the file.\n";
		return DECODER_ERROR_FILE;
	}
	int audio_stream = -1;
	for (int i = 0; i < (int)formatCtx->nb_streams; i++) {
		if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audio_stream = i;
			break;
		}
	}
	if (audio_stream == -1) {
		avformat_close_input(&formatCtx);
		cout << "AV Error! " << "Could not find an audio stream in the file.\n";
		return DECODER_ERROR_FILE;
	}
	
	// Determine the track duration.
	double duration_secs = formatCtx->duration / AV_TIME_BASE;
	double duration_mins = duration_secs / 60;
	// (From is_KeyFinder) First condition is a hack for bizarre overestimation
	// of some MP3s.
	if (duration_mins < 720 && duration_secs > maxDuration * 60) {
		avformat_close_input(&formatCtx);
		cout << "AV Error! Duration of file " << duration_mins << ":" << duration_secs << " exceeds specified maximum " << maxDuration << ".\n";
		return DECODER_ERROR_FILE;
	}
	
	// Determine the stream codec.
	codecCtx = formatCtx->streams[audio_stream]->codec;
	codec = avcodec_find_decoder(codecCtx->codec_id);
	if (codec == NULL) {
		avformat_close_input(&formatCtx);
		cout << "AV Error! Audio stream has an unsupported codec.\n";
		return DECODER_ERROR_FILE;
	}
	
	// Open the codec.
	int codec_open_result = avcodec_open2(codecCtx, codec, &dict);
	if (codec_open_result < 0) {
		char *errbuf = (char*)malloc(50 * sizeof(char));
		av_strerror(codec_open_result, errbuf, 50);
		avformat_close_input(&formatCtx);
		cout << "AV Error! " << errbuf << " (" << codec_open_result << ").\n";
		return DECODER_ERROR_WITH_AV(codec_open_result);
	}
	
	ReSampleContext *resample = av_audio_resample_init(codecCtx->channels, codecCtx->channels, codecCtx->sample_rate, codecCtx->sample_rate, AV_SAMPLE_FMT_S16, codecCtx->sample_fmt, 0, 0, 0, 0);
	if (resample == NULL) {
		avcodec_close(codecCtx);
		avformat_close_input(&formatCtx);
		cout << "AV Error! Could not create the resample context.\n";
		return DECODER_ERROR_FILE;
	}
	
	cout << "Decoding \"" << path << "\" (" << av_get_sample_fmt_name(codecCtx->sample_fmt) << ", " << codecCtx->sample_rate << "Hz, " << (int)codecCtx->bit_rate << "bps).\n";
	
	
	//--------------------------------------------------------//
	// Create the audio object!
	//--------------------------------------------------------//
	KeyFinder::AudioData audio;
	audio.setFrameRate(codecCtx->sample_rate);
	audio.setChannels(codecCtx->channels);
	
	// Decode stream.
	AVPacket packet;
	int bad_packet_count = 0;
	int bad_packet_threshold = 100;
	while (true) {
		av_init_packet(&packet);
		if (av_read_frame(formatCtx, &packet) < 0) {
			break;
		}
		if (packet.stream_index == audio_stream) {
			try {
				int result = decodePacket(codecCtx, resample, &packet, audio);
				if (result != 0) {
					if (bad_packet_count < bad_packet_threshold) {
						bad_packet_threshold++;
					} else {
						avcodec_close(codecCtx);
						avformat_close_input(&formatCtx);
						cout << "AV Error! Too many bad packets (" << bad_packet_count << ") while decoding file.\n";
						throw KeyFinder::Exception(DECODER_ERROR_PACKET_THRESHOLD);
					}
				}
			} catch (KeyFinder::Exception& e) {
				cout << "Encountered KeyFinder::Exception (" << e.what() << ") while decoding file.\n";
				throw e;
			}
		}
		av_free_packet(&packet);
	}
	
	audio_resample_close(resample);
	int codecCloseResult = avcodec_close(codecCtx);
	if (codecCloseResult < 0) {
		cout << "Error closing audio codec: " << codec->long_name << " (" << codecCloseResult << ")\n";
		return DECODER_ERROR_WITH_AV(codecCloseResult);
	}
	
	avformat_close_input(&formatCtx);
	
	audioData = audio;
	
	return 0;
}


