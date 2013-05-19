/*************************************************************************
 
 Copyright 2011-2013 Ibrahim Sha'ath
 
 This file is part of KeyFinder.
 
 KeyFinder is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 KeyFinder is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with KeyFinder.  If not, see <http://www.gnu.org/licenses/>.
 
 *************************************************************************/

#include "strings.h"

std::string appName(){
	return "KeyFinder";
}

std::string appUrl(){
	return "http://www.ibrahimshaath.co.uk/keyfinder/";
}

std::string delim(){
	return " - ";
}

std::string notApplicable(){
	//: Status of an individual file in the Batch window
	return "N/A";
}

std::string libsrcError(){
	//: Status of an individual file in the Batch window
	return "Error in LibSRC sample rate conversion";
}

std::string libavCouldNotOpenFile(int n){
	//: Status of an individual file in the Batch window
	std::stringstream ss;
	ss << "Could not open audio file (" << n << ")";
	return ss.str();
}

std::string libavCouldNotFindStreamInformation(){
	//: Status of an individual file in the Batch window
	return "Could not find stream information";
}

std::string libavCouldNotFindAudioStream(){
	//: Status of an individual file in the Batch window
	return "Could not find an audio stream";
}

std::string libavUnsupportedCodec(){
	//: Status of an individual file in the Batch window
	return "Audio stream has unsupported codec";
}

std::string libavCouldNotOpenCodec(const char* name, int result){
	//: Status of an individual file in the Batch window
	std::stringstream ss;
	ss << "Could not open audio codec " << name << " (" << result << ")";
	return ss.str();
}

std::string libavCouldNotCreateResampleContext(){
	//: Status of an individual file in the Batch window
	return "Could not create ReSampleContext";
}

std::string libavCouldNotResample(){
	//: Status of an individual file in the Batch window
	return "Could not resample";
}

std::string libavTooManyBadPackets(int n){
	//: Status of an individual file in the Batch window
	std::stringstream ss;
	ss << n << " bad packet(s)";
	return ss.str();
}

std::string durationExceedsPreference(int mins, int secs, int max){
	//: Status of an individual file in the Batch window; includes minutes and seconds at %1:%2, and the max number of minutes at %3
	std::stringstream ss;
	ss << "Duration (" << mins << ":" << secs << ") exceeds specified maximum (" << max << ":00)";
	return ss.str();
}
