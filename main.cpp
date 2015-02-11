//
//  main.cpp
//  soundplayer
//
//  Created by the DtTvB on 4/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <fmod_errors.h>
#include <fmod.hpp>

FMOD_RESULT result;
FMOD::System *fmod;

class Sample {
	std::string filename;
	FMOD::Sound *sound;
public:
	Sample(const std::string &filename);
	bool init();
	bool play(float volume);
};

Sample::Sample(const std::string &filename) {
	this->filename = filename;
}

bool Sample::init() {
	result = fmod->createSound(this->filename.c_str(), FMOD_DEFAULT, 0, &(this->sound));
	if (result != FMOD_OK) {
		return false;
	}
	return true;
}

bool Sample::play(float volume) {
	FMOD::Channel *channel;
	fmod->update();
	result = fmod->playSound(FMOD_CHANNEL_FREE, this->sound, true, &channel);
	if (result != FMOD_OK) {
		return false;
	}
	result = channel->setVolume(volume);
	result = channel->setPaused(false);
	return true;
}

typedef std::map<std::string, Sample *> SampleMap;
SampleMap samples;

Sample *getSample(const std::string &filename) {
	
	SampleMap::iterator iter = samples.find(filename);
	if (iter != samples.end()) {
		return (*iter).second;
	}
	
	fprintf(stderr, "Loading sample %s\n", filename.c_str());
	
	Sample *sample = new Sample(filename);
	if (!sample->init()) {
		fprintf(stderr, "... Cannot load sample!\n");
		delete sample;
		sample = NULL;
	}
	
	samples[filename] = sample;
	return sample;

}

int main(int argc, const char * argv[])
{
	
	result = FMOD::System_Create(&fmod);
	if (result != FMOD_OK) {
		fprintf(stderr, "Cannot create fmod! %d: %s\n", result, FMOD_ErrorString(result));
		return 1;
	}
	
	result = fmod->init(100, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		fprintf(stderr, "Cannot init fmod! %d: %s\n", result, FMOD_ErrorString(result));
		return 1;
	}
	
	char input[1024];
	float nextVolume = 1.0;
	while (!feof(stdin)) {
		fgets(input, 1024, stdin);
		for (int i = 0; i < 1024; i ++) {
			if (input[i] == '\r' || input[i] == '\n') {
				input[i] = '\0';
				break;
			}
		}
		char *filename = input + 1;
		std::string fn = filename;
		char operation = input[0];
		if (operation == 'l') {
			getSample(fn);
		} else if (operation == 'v') {
			nextVolume = (float)atof(input + 1);
		} else if (operation == 's') {
			FMOD::ChannelGroup *channelgroup;
			result = fmod->getMasterChannelGroup(&channelgroup);
			if (result == FMOD_OK) {
				channelgroup->stop();
			}
		} else if (operation == 'p') {
			Sample *sample = getSample(fn);
			if (sample != NULL) {
				fprintf(stderr, "Playing sample %s\n", filename);
				if (!sample->play(nextVolume)) {
					fprintf(stderr, "... Cannot play sample!\n");
				}
			}
		}
	}
    return 0;
}

