//
//  keycodes.cpp
//  keyfinder
//
//  Created by James Reuss on 19/05/2013.
//  Copyright (c) 2013 Darksky. All rights reserved.
//

#include "keycodes.h"

KeyCodes::KeyCodes()
{
	
	// ========================= Default Key Codes ==============================
	
	defaultKeyCodes.push_back("A");		defaultKeyCodes.push_back("Am");
	defaultKeyCodes.push_back("Bb");	defaultKeyCodes.push_back("Bbm");
	defaultKeyCodes.push_back("B");		defaultKeyCodes.push_back("Bm");
	defaultKeyCodes.push_back("C");		defaultKeyCodes.push_back("Cm");
	defaultKeyCodes.push_back("Db");	defaultKeyCodes.push_back("Dbm");
	defaultKeyCodes.push_back("D");		defaultKeyCodes.push_back("Dm");
	defaultKeyCodes.push_back("Eb");	defaultKeyCodes.push_back("Ebm");
	defaultKeyCodes.push_back("E");		defaultKeyCodes.push_back("Em");
	defaultKeyCodes.push_back("F");		defaultKeyCodes.push_back("Fm");
	defaultKeyCodes.push_back("Gb");	defaultKeyCodes.push_back("Gbm");
	defaultKeyCodes.push_back("G");		defaultKeyCodes.push_back("Gm");
	defaultKeyCodes.push_back("Ab");	defaultKeyCodes.push_back("Abm");
	defaultKeyCodes.push_back("...");
	
	// ========================== Custom Key Codes ===============================
	
	/*if (0) {
		customKeyCodes.erase(customKeyCodes.begin(), customKeyCodes.end());
		customKeyCodes.push_back("A");
		customKeyCodes.push_back("Am");
		customKeyCodes.push_back("Bb");
		customKeyCodes.push_back("Bbm");
		customKeyCodes.push_back("B");
		customKeyCodes.push_back("Bm");
		customKeyCodes.push_back("C");
		customKeyCodes.push_back("Cm");
		customKeyCodes.push_back("Db");
		customKeyCodes.push_back("Dbm");
		customKeyCodes.push_back("D");
		customKeyCodes.push_back("Dm");
		customKeyCodes.push_back("Eb");
		customKeyCodes.push_back("Ebm");
		customKeyCodes.push_back("E");
		customKeyCodes.push_back("Em");
		customKeyCodes.push_back("F");
		customKeyCodes.push_back("Fm");
		customKeyCodes.push_back("Gb");
		customKeyCodes.push_back("Gbm");
		customKeyCodes.push_back("G");
		customKeyCodes.push_back("Gm");
		customKeyCodes.push_back("Ab");
		customKeyCodes.push_back("Abm");
		customKeyCodes.push_back("SLNC");
	} else {
		customKeyCodes.erase(customKeyCodes.begin(), customKeyCodes.end());
		for(int i=0; i<25; i++)
			customKeyCodes.push_back("");
	}*/
	
}

string KeyCodes::getKeyCode(int code) const
{
	if (code < 0 || code >= defaultKeyCodes.size()) {
		cout << "Attempt to get name of out-of-bounds key (" << code << "/" << defaultKeyCodes.size() << ")\n";
		return NULL;
	}
	
	string keyCode = defaultKeyCodes[code];
	return keyCode;
}
