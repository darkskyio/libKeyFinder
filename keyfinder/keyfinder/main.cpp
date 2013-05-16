//
//  main.cpp
//  keyfinder
//
//  Created by James Reuss on 15/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#define MAX_SONG_LENGTH 10	// mins

#include <iostream>

extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace std;

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		cout << "You entered no input! Exiting.\n";
		return -1;
	}
	
	string path;
	path = argv[1];
	
	//--------------------------------------------------------//
	// Decode the audio!
	//--------------------------------------------------------//
	av_register_all();
	
	AVFormatContext *av_fcontext = NULL;
	AVCodecContext	*av_ccontext = NULL;
	AVCodec *av_codec = NULL;
	AVDictionary *av_dict = NULL;
	const char* av_filename = path.c_str();
	
	// Open the file.
	int open_input_result = avformat_open_input(&av_fcontext, av_filename, NULL, NULL);
	if (open_input_result != 0) {
		char *errbuf = (char*)malloc(50 * sizeof(char));
		av_strerror(open_input_result, errbuf, 50);
		cout << "AV Error! " << errbuf << " (" << open_input_result << ") Exiting.\n";
		return open_input_result;
	}
	
	printf("FLAG: Open complete.\n");
	
	// Get the stream info for the audio.
	if (avformat_find_stream_info(av_fcontext, NULL) < 0) {
		avformat_close_input(&av_fcontext);
		cout << "AV Error! " << "Could not find the stream information for the file. Exiting.\n";
		return -1;
	}
	printf("FLAG: Get stream pt1.\n");
	int audio_stream = -1;
	for (int i = 0; i < (int)av_fcontext->nb_streams; i++) {
		printf("\tnext stream.\n");
		if (av_fcontext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audio_stream = i;
			break;
		}
	}
	printf("FLAG: Get stream pt2.\n");
	if (audio_stream == -1) {
		avformat_close_input(&av_fcontext);
		cout << "AV Error! " << "Could not find an audio stream in the file. Exiting.\n";
		return -1;
	}
	
	printf("FLAG: Get stream complete.\n");
	
	// Determine the track duration.
	double duration_secs = av_fcontext->duration / AV_TIME_BASE;
	double duration_mins = duration_secs / 60;
	// (From is_KeyFinder) First condition is a hack for bizarre overestimation
	// of some MP3s.
	if (duration_mins < 720 && duration_secs > MAX_SONG_LENGTH * 60) {
		avformat_close_input(&av_fcontext);
		cout << "AV Error! Duration of file " << duration_mins << ":" << duration_secs << " exceeds specified maximum " << MAX_SONG_LENGTH << ". Exiting.\n";
		return -1;
	}
	
	printf("FLAG: Determine duration complete.\n");
	
	// Determine the stream codec.
	av_ccontext = av_fcontext->streams[audio_stream]->codec;
	av_codec = avcodec_find_decoder(av_ccontext->codec_id);
	if (av_codec == NULL) {
		avformat_close_input(&av_fcontext);
		cout << "AV Error! Audio stream has an unsupported codec. Exiting.\n";
		return -1;
	}
	
	printf("FLAG: Determine codec complete.\n");
	
	// Open the codec.
	int codec_open_result = avcodec_open2(av_ccontext, av_codec, &av_dict);
	if (codec_open_result < 0) {
		char *errbuf = (char*)malloc(50 * sizeof(char));
		av_strerror(codec_open_result, errbuf, 50);
		avformat_close_input(&av_fcontext);
		cout << "AV Error! " << errbuf << " (" << codec_open_result << ") Exiting.\n";
		return codec_open_result;
	}
	
	printf("FLAG: Open codec complete.\n");
	
	ReSampleContext *resample = av_audio_resample_init(av_ccontext->channels, av_ccontext->channels, av_ccontext->sample_rate, av_ccontext->sample_rate, AV_SAMPLE_FMT_S16, av_ccontext->sample_fmt, 0, 0, 0, 0);
	if (resample == NULL) {
		avcodec_close(av_ccontext);
		avformat_close_input(&av_fcontext);
		cout << "AV Error! Could not create the resample context. Exiting.\n";
		return -1;
	}
	
	cout << "Decoding \"" << path << "\" (" << av_get_sample_fmt_name(av_ccontext->sample_fmt) << ", " << av_ccontext->sample_rate << ").\n";
	
	
	
	
	
	// All done.
	cout << "Done with file \"" << path << "\".\n";
    return 0;
}

