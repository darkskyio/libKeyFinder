//
//  keycodes.h
//  keyfinder
//
//  Created by James Reuss on 19/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#ifndef __keyfinder__keycodes__
#define __keyfinder__keycodes__

#include <iostream>
#include <vector>

using namespace std;

class KeyCodes {
	
	//vector<string> customKeyCodes;
	vector<string> defaultKeyCodes;
	
public:
	
	KeyCodes();
	//~KeyCodes();
	string getKeyCode(int) const;
	
};

#endif /* defined(__keyfinder__keycodes__) */
