//
//  main.cpp
//  keyfinder
//
//  Created by James Reuss on 15/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#define MAX_SONG_LENGTH 60	// mins

#include <iostream>
#include "keyfinder.h"
#include "asyncfileobject.h"
#include "asynckeyprocess.h"
#include "keycodes.h"

using namespace std;

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		cout << "FAIL." << endl;
		return -1;
	}
	
	string path;
	path = argv[1];
	
	AsyncFileObject object(path, 0);
	KeyFinderResultWrapper result = keyDetectionProcess(object);
	if (result.errorMessage.size() > 0) {
		std::cerr << result.errorMessage << endl;
		return 1;
	}
	
	KeyCodes codes;
	std::cout << codes.getKeyCode(result.core.globalKeyEstimate) << endl;
	
    return 0;
}

