//
//  decoder.h
//  keyfinder
//
//  Created by James Reuss on 16/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#ifndef __keyfinder__decoder__
#define __keyfinder__decoder__

#include <iostream>
extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "audiodata.h"

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

#define DECODER_ERROR_WITH_AV(x) (-100-x)
#define DECODER_ERROR_FILE -1;
#define DECODER_ERROR_PACKET -2;
#define DECODER_ERROR_PACKET_RESAMPLE "libav could not resample the audio data."
#define DECODER_ERROR_PACKET_THRESHOLD "libav audio contained too many bad packets."

class Decoder {
	
	uint8_t* frameBuffer;
	uint8_t* frameBufferConverted;
	int frameBufferSize;
	
	int decodePacket(AVCodecContext *codec, ReSampleContext *resample, AVPacket* originalPacket, KeyFinder::AudioData &audio);
	
public:
	
	Decoder();
	~Decoder();
	int decodeFile(std::string path, const int maxDuration, KeyFinder::AudioData &audioData);
	
};

#endif /* defined(__keyfinder__decoder__) */
