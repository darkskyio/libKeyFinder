//
//  main.cpp
//  keyfinder
//
//  Created by James Reuss on 15/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#define MAX_SONG_LENGTH 10	// mins

#include <iostream>
#include "keyfinder.h"
#include "decoder.h"
#include "keytostring.h"

using namespace std;

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		cout << "You entered no input! Exiting.\n";
		return -1;
	}
	
	string path;
	path = argv[1];
	
	// Keep these things for as long as we need them.
	static Decoder decoder;
	static KeyFinder::KeyFinder keyFinder;
	static KeyToString keyString;
	keyString.setKeyType(MUSICAL);
	
	// This is only relevant for the current song to be processed.
	KeyFinder::AudioData audio;
	
	int decoderResult = decoder.decodeFile(path, MAX_SONG_LENGTH, audio);
	if (decoderResult < 0) {
		return decoderResult;
	}
	KeyFinder::KeyDetectionResult keyResult = keyFinder.keyOfAudio(audio);
	
	
	
	cout << "Key found to be: " << keyString.getString(keyResult.globalKeyEstimate) << ".\n";
	
//	// Run the analysis
//	KeyFinder::KeyDetectionResult r =  k.keyOfAudio(a);
//	
//	// And do something with the result!
//	doSomethingWith(r.globalKeyEstimate);
	
	// All done.
	cout << "Done with file \"" << path << "\".\n";
    return 0;
}

