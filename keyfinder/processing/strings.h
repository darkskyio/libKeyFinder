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

#ifndef STRINGS_H
#define STRINGS_H

#include <iostream>
#include <sstream>

std::string appName();
std::string appUrl();
std::string delim();
std::string notApplicable();
std::string libsrcError();
std::string libavCouldNotOpenFile(int);
std::string libavCouldNotFindStreamInformation();
std::string libavCouldNotFindAudioStream();
std::string libavUnsupportedCodec();
std::string libavCouldNotOpenCodec(const char*, int);
std::string libavCouldNotCreateResampleContext();
std::string libavCouldNotResample();
std::string libavTooManyBadPackets(int);
std::string durationExceedsPreference(int, int, int);

#endif // STRINGS_H
