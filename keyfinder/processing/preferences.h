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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <iostream>

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cstring>

#include "parameters.h"
#include "strings.h"
#include "constants.h"

enum metadata_format_t{
	METADATA_FORMAT_KEYS,
	METADATA_FORMAT_CUSTOM,
	METADATA_FORMAT_BOTH
};

enum metadata_write_t{
	METADATA_WRITE_NONE,
	METADATA_WRITE_PREPEND,
	METADATA_WRITE_APPEND,
	METADATA_WRITE_OVERWRITE
};

enum chromagram_colour_t{
	CHROMA_COLOUR_IZO,
	CHROMA_COLOUR_MONO,
	CHROMA_COLOUR_PRED,
	CHROMA_COLOUR_VBOY,
	CHROMA_COLOUR_HACK
};

class Preferences{
public:
	KeyFinder::Parameters core;
	Preferences();
	Preferences& operator=(const Preferences&);
	
	// accessors
	bool getWriteToFilesAutomatically() const;
	bool getParallelBatchJobs() const;
	int getMaxDuration() const;
	std::vector<std::string> getCustomKeyCodes() const;
	
	// accessors tied to KeyFinder::Parameters object
	bool getOffsetToC() const;
	KeyFinder::temporal_window_t getTemporalWindow() const;
	KeyFinder::segmentation_t getSegmentation() const;
	KeyFinder::similarity_measure_t getSimilarityMeasure() const;
	KeyFinder::tuning_method_t getTuningMethod() const;
	KeyFinder::tone_profile_t getToneProfile() const;
	unsigned int getFftFrameSize() const;
	unsigned int getHopsPerFrame() const;
	unsigned int getOctaves() const;
	unsigned int getBpo() const;
	unsigned int getArbitrarySegments() const;
	unsigned int getSegPeakPickingNeighbours() const;
	unsigned int getSegGaussianSize() const;
	float getSegGaussianSigma() const;
	float getStartingFreqA() const;
	float getLastFreq() const;
	float getDirectSkStretch() const;
	float getDetunedBandWeight() const;
	std::vector<float> getCustomToneProfile() const;
	
	// mutators
	void setMaxDuration(int);
	void setCustomKeyCodes(const std::vector<std::string>&);
	
	// mutators tied to KeyFinder::Parameters object
	void setOffsetToC(bool);
	void setTemporalWindow(KeyFinder::temporal_window_t);
	void setSegmentation(KeyFinder::segmentation_t);
	void setSimilarityMeasure(KeyFinder::similarity_measure_t);
	void setTuningMethod(KeyFinder::tuning_method_t);
	void setToneProfile(KeyFinder::tone_profile_t);
	void setFftFrameSize(unsigned int);
	void setHopsPerFrame(unsigned int);
	void setOctaves(unsigned int);
	void setBps(unsigned int);
	void setArbitrarySegments(unsigned int);
	void setSegPeakPickingNeighbours(unsigned int);
	void setSegGaussianSize(unsigned int);
	void setSegGaussianSigma(float);
	void setStartingFreqA(float);
	void setDirectSkStretch(float);
	void setDetunedBandWeight(float);
	void setCustomToneProfile(const std::vector<float>&);
	
	// not necessarily related to user preferences, just here for convenience
	std::string getKeyCode(int) const;
	std::vector<std::string> getKeyCodeList() const;
	
private:
	
	int maxDuration;
	std::vector<std::string> customKeyCodes;
	std::vector<std::string> defaultKeyCodes;
	
	int scaledSine(int, float, float, int, int) const;
};

#endif
