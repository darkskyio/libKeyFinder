//
//  main.cpp
//  keyfinder
//
//  Created by James Reuss on 15/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

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
	
	// Decode the audio.
	av_register_all();
	
	AVFormatContext *av_fcontext = NULL;
	const char* av_filename = path.c_str();
	
	int open_input_result = avformat_open_input(&av_fcontext, av_filename, NULL, NULL);
	if (open_input_result != 0) {
		char *errbuf = (char*)malloc(50 * sizeof(char));
		av_strerror(open_input_result, errbuf, 50);
		cout << "AV Error! " << errbuf << " (" << open_input_result << ") Exiting.\n";
		return open_input_result;
	}

	// insert code here...
	cout << "Done with file \"" << path << "\".\n";
    return 0;
}

