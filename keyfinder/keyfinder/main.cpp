//
//  main.cpp
//  keyfinder
//
//  Created by James Reuss on 15/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#define MAX_SONG_LENGTH 10	// mins

#include <iostream>
#include "decoder.h"

using namespace std;

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		cout << "You entered no input! Exiting.\n";
		return -1;
	}
	
	string path;
	path = argv[1];
	
	
	Decoder decoder;
	KeyFinder::AudioData audio;
	
	decoder.decodeFile(path, MAX_SONG_LENGTH, &audio);
	//keyfind now
	
	
	// All done.
	cout << "Done with file \"" << path << "\".\n";
    return 0;
}

