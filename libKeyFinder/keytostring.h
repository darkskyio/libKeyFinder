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
	
	const std::vector<std::string> musical_keys = {
		"A",	// A_MAJOR
		"Am",	// A_MINOR
		"Bb",	// B_FLAT_MAJOR
		"Bbm",	// B_FLAT_MINOR
		"B",	// B_MAJOR
		"Bm",	// B_MINOR
		"C",	// C_MAJOR
		"Cm",	// C_MINOR
		"Dbm",	// D_FLAT_MAJOR
		"Db",	// D_FLAT_MINOR
		"D",	// D_MAJOR
		"Dm",	// D_MINOR
		"Eb",	// E_FLAT_MAJOR
		"Ebm",	// E_FLAT_MINOR
		"E",	// E_MAJOR
		"Em",	// E_MINOR
		"F",	// F_MAJOR
		"Fm",	// F_MINOR			Db
		"Gb",	// G_FLAT_MAJOR
		"Gbm",	// G_FLAT_MINOR
		"G",	// G_MAJOR
		"Gm",	// G_MINOR			Db
		"Ab",	// A_FLAT_MAJOR
		"Abm",	// A_FLAT_MINOR
		"None"	// SILENCE
	};
	
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
