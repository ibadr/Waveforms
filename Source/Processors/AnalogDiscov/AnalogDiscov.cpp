/*
 ------------------------------------------------------------------

 This file is part of the Open Ephys GUI
 Copyright (C) 2014 Florian Franzen

 ------------------------------------------------------------------

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */


#include <stdio.h>
#include "AnalogDiscov.h"

AnalogDiscov::AnalogDiscov()
    : GenericProcessor("Analog Discovery")
{
	timestamp = 0;
	currentNumChannels = 10;
}

AnalogDiscov::~AnalogDiscov()
{
}

bool AnalogDiscov::isReady()
{
    return true;
}

bool AnalogDiscov::disable()
{
    return true;
}


void AnalogDiscov::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
	setTimestamp(events, timestamp);
	
	int samplesNeeded = 100;// (int) float(buffer.getNumSamples()) * (getDefaultSampleRate() / 44100.0f);
	// FIXME: needs to account for the fact that the ratio might not be an exact
	//        integer value

	for (int ch = 0; ch < currentNumChannels; ch++)
	{
		float* outBuffer = buffer.getWritePointer(ch, 0);
		for (int i = 0; i < samplesNeeded; i++)
		{
			*(outBuffer + i) = 100 * (ch + 1) / (getDefaultSampleRate() / 4)*((timestamp + i) % ((int)getDefaultSampleRate() / 4));
		}
	}

	timestamp += samplesNeeded;
	setNumSamples(events, samplesNeeded);
}

AudioProcessorEditor* AnalogDiscov::createEditor()
{
    editor = new AnalogDiscovEditor(this);
    return editor;
}

float AnalogDiscov::getDefaultSampleRate()
{
	return 30000.0f;
}

int AnalogDiscov::getNumHeadstageOutputs()
{
	return currentNumChannels;
}