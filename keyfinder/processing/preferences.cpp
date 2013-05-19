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

#include "preferences.h"

Preferences::Preferences(){
	
	maxDuration = 15;
	
	// ======================= Musical frequencies ===============================
	
	// Bear in mind that KeyFinder params object has built in defaults
	
	core.setStartingFrequencyA(27.5);
	core.setOctaves(8);
	core.setOffsetToC(true);
	core.setBandsPerSemitone(1);
	
	// ======================== Spectral analysis ================================
	
	core.setTemporalWindow(KeyFinder::WINDOW_BLACKMAN);
	core.setFftFrameSize(16384);
	core.setHopsPerFrame(4096);
	core.setDirectSkStretch(0.8);
	
	// ============================= Tuning ======================================
	
	core.setTuningMethod(KeyFinder::TUNING_HARTE);
	core.setDetunedBandWeight(0.2);
	
	// =========================== Segmentation ==================================
	
	core.setSegmentation(KeyFinder::SEGMENTATION_NONE);
	core.setSegGaussianSize(35);
	core.setSegGaussianSigma(8.0);
	core.setSegPeakPickingNeighbours(4);
	core.setArbitrarySegments(3);
	
	// ========================= Key classification ==============================
	
	core.setToneProfile(KeyFinder::TONE_PROFILE_SHAATH);
	core.setSimilarityMeasure(KeyFinder::SIMILARITY_COSINE);
	
	// ========================= Custom Tone Profile ==============================
	
	std::vector<float> ctp;
	// Tone profiles are stored sequentially; major chroma vector then minor chroma vector
	// Major
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	// Minor
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	ctp.push_back(1.00);
	ctp.push_back(0.00);
	try{
		core.setCustomToneProfile(ctp);
	}catch(std::exception& e){
		printf("LibKeyFinder Exception: %s\n", e.what());
	}catch(...){
		printf("Unknown exception setting custom tone profile.\n");
	}
	
	// ========================== Custom Key Codes ===============================
	
	if (0) {
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
	}
	
	// ================================ End ======================================
	
	// default key codes
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
}

Preferences& Preferences::operator=(const Preferences& that){
	if(this != &that){
		core = that.core;
		customKeyCodes = that.customKeyCodes;
		maxDuration = that.maxDuration;
	}
	return *this;
}

int               Preferences::getMaxDuration()               const { return maxDuration; }

bool                            Preferences::getOffsetToC()                const { return core.getOffsetToC(); }
KeyFinder::temporal_window_t    Preferences::getTemporalWindow()           const { return core.getTemporalWindow(); }
KeyFinder::segmentation_t       Preferences::getSegmentation()             const { return core.getSegmentation(); }
KeyFinder::similarity_measure_t Preferences::getSimilarityMeasure()        const { return core.getSimilarityMeasure(); }
KeyFinder::tuning_method_t      Preferences::getTuningMethod()             const { return core.getTuningMethod(); }
KeyFinder::tone_profile_t       Preferences::getToneProfile()              const { return core.getToneProfile(); }
unsigned int                    Preferences::getFftFrameSize()             const { return core.getFftFrameSize(); }
unsigned int                    Preferences::getHopsPerFrame()             const { return core.getHopsPerFrame(); }
unsigned int                    Preferences::getOctaves()                  const { return core.getOctaves(); }
unsigned int                    Preferences::getBpo()                      const { return core.getBandsPerOctave(); }
unsigned int                    Preferences::getArbitrarySegments()        const { return core.getArbitrarySegments(); }
unsigned int                    Preferences::getSegPeakPickingNeighbours() const { return core.getSegPeakPickingNeighbours(); }
unsigned int                    Preferences::getSegGaussianSize()          const { return core.getSegGaussianSize(); }
float                           Preferences::getSegGaussianSigma()         const { return core.getSegGaussianSigma(); }
float                           Preferences::getStartingFreqA()            const { return core.getStartingFreqA(); }
float                           Preferences::getLastFreq()                 const { return core.getLastFrequency(); }
float                           Preferences::getDirectSkStretch()          const { return core.getDirectSkStretch(); }
float                           Preferences::getDetunedBandWeight()        const { return core.getDetunedBandWeight(); }
std::vector<float>              Preferences::getCustomToneProfile()        const { return core.getCustomToneProfile(); }

void Preferences::setMaxDuration(int max)                          { maxDuration = max; }

void Preferences::setOffsetToC(bool off)                                  { core.setOffsetToC(off); }
void Preferences::setTemporalWindow(KeyFinder::temporal_window_t w)       { core.setTemporalWindow(w); }
void Preferences::setSegmentation(KeyFinder::segmentation_t h)            { core.setSegmentation(h); }
void Preferences::setSimilarityMeasure(KeyFinder::similarity_measure_t m) { core.setSimilarityMeasure(m); }
void Preferences::setTuningMethod(KeyFinder::tuning_method_t t)           { core.setTuningMethod(t); }
void Preferences::setToneProfile(KeyFinder::tone_profile_t profile)       { core.setToneProfile(profile); }
void Preferences::setHopsPerFrame(unsigned int hpf)                       { core.setHopsPerFrame(hpf); }
void Preferences::setFftFrameSize(unsigned int framesize)                 { core.setFftFrameSize(framesize); }
void Preferences::setOctaves(unsigned int oct)                            { core.setOctaves(oct); }
void Preferences::setBps(unsigned int bands)                              { core.setBandsPerSemitone(bands); }
void Preferences::setArbitrarySegments(unsigned int segments)             { core.setArbitrarySegments(segments); }
void Preferences::setSegPeakPickingNeighbours(unsigned int neighbours)    { core.setSegPeakPickingNeighbours(neighbours); }
void Preferences::setSegGaussianSize(unsigned int size)                   { core.setSegGaussianSize(size); }
void Preferences::setSegGaussianSigma(float sigma)                        { core.setSegGaussianSigma(sigma); }
void Preferences::setStartingFreqA(float a)                               { core.setStartingFrequencyA(a); }
void Preferences::setDirectSkStretch(float stretch)                       { core.setDirectSkStretch(stretch); }
void Preferences::setDetunedBandWeight(float weight)                      { core.setDetunedBandWeight(weight); }
void Preferences::setCustomToneProfile(const std::vector<float>& profile) { core.setCustomToneProfile(profile); }


std::string Preferences::getKeyCode(int n) const{
	if(n < 0 || n >= defaultKeyCodes.size()){
		printf("Attempt to get name of out-of-bounds key (%d/%d)\n", n, (int)defaultKeyCodes.size());
		return "";
	}
	std::string defaultCode = defaultKeyCodes[n];
	std::string customCode = customKeyCodes[n];
	//if(metadataFormat == METADATA_FORMAT_KEYS || customCode == "")
		return defaultCode;
	//else if(metadataFormat == METADATA_FORMAT_CUSTOM)
	//	return customCode;
	//else
	//	return customCode + " " + defaultCode;
}

std::vector<std::string> Preferences::getKeyCodeList() const{
	std::vector<std::string> output;
	for(int i = 0; i < defaultKeyCodes.size(); i++)
		output.push_back(getKeyCode(i));
	return output;
}

int Preferences::scaledSine(int index, float xScale, float xOffset, int yScale, int yOffset) const {
	int result = yScale * sin((index * xScale) + xOffset) + yOffset;
	if (result <   0) return 0;
	if (result > 255) return 255;
	return result;
}
