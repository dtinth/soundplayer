//
//  main.cpp
//  soundplayer
//
//  Created by the DtTvB on 4/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "bass.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

typedef std::map<std::string, HSAMPLE> SampleMap;

SampleMap samples;

HSAMPLE getSample(const std::string &filename) {
	SampleMap::iterator iter = samples.find(filename);
	if (iter != samples.end()) {
		return (*iter).second;
	}
	fprintf(stderr, "Loading sample %s\n", filename.c_str());
	HSAMPLE sample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 2, BASS_SAMPLE_OVER_POS);
	if (sample == 0) {
		fprintf(stderr, "... Cannot load sample!\n");
	}
	samples[filename] = sample;
	return sample;
}

int main(int argc, const char * argv[])
{
	if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
		fprintf(stderr, "Cannot start bass!\n");
		return 1;
	}
	char input[1024];
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
		} else if (operation == 's') {
			BASS_Stop();
			BASS_Start();
		} else if (operation == 'p') {
			HSAMPLE sample = getSample(fn);
			if (sample != 0) {
				HCHANNEL channel = BASS_SampleGetChannel(sample, FALSE);
				fprintf(stderr, "Playing sample %s\n", filename);
				if (channel != 0) {
					BASS_ChannelPlay(channel, TRUE);
				} else {
					fprintf(stderr, "... Cannot play sample!\n");
				}
			}
		}
	}
    return 0;
}

