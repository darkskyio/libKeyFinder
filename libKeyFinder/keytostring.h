//
//  keytostring.h
//  keyfinder
//
//  Created by James Reuss on 18/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#ifndef keyfinder_keytostring_h
#define keyfinder_keytostring_h

#include <iostream>
#include "constants.h"

enum key_type {
	MUSICAL = 0
};

class KeyToString {
	
	std::vector<std::string> musical_keys;
	
	key_type type;
	
public:
	
	KeyToString();
	~KeyToString();
	
	void setKeyType(key_type aType);
	
	std::string getString(key_t key);
	
};

KeyToString::KeyToString()
{
	type = MUSICAL;
	
	musical_keys.push_back("A");	musical_keys.push_back("Am");
	musical_keys.push_back("Bb");	musical_keys.push_back("Bbm");
	musical_keys.push_back("B");	musical_keys.push_back("Bm");
	musical_keys.push_back("C");	musical_keys.push_back("Cm");
	musical_keys.push_back("Db");	musical_keys.push_back("Dbm");
	musical_keys.push_back("D");	musical_keys.push_back("Dm");
	musical_keys.push_back("Eb");	musical_keys.push_back("Ebm");
	musical_keys.push_back("E");	musical_keys.push_back("Em");
	musical_keys.push_back("F");	musical_keys.push_back("Fm");
	musical_keys.push_back("Gb");	musical_keys.push_back("Gbm");
	musical_keys.push_back("G");	musical_keys.push_back("Gm");
	musical_keys.push_back("Ab");	musical_keys.push_back("Abm");
	musical_keys.push_back("...");
}
KeyToString::~KeyToString() {}

void KeyToString::setKeyType(key_type aType)
{
	type = aType;
}

std::string KeyToString::getString(key_t key) {
	std::string string;
	
	switch (type) {
		case MUSICAL:
			string = musical_keys.at(key);
			break;
			
		default:
			break;
	}
	
	return string;
}

#endif
