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

#include <stdio.h>
#include <stdlib.h>

AnalogDiscov::AnalogDiscov()
    : GenericProcessor("Analog Discovery")
{
	timestamp = 0;
	_currentNumChannels = 10;

	int cChannel;

	printf("Open automatically the first available device\n");
	FDwfDeviceOpen(-1, &_hdwf);

	// get the number of analog in channels
	FDwfAnalogInChannelCount(_hdwf, &cChannel);
	_currentNumChannels = cChannel;

	// enable channels
	for (int c = 0; c < cChannel; c++){
		FDwfAnalogInChannelEnableSet(_hdwf, c, true);
	}
	// set 15V pk2pk input range for all channels
	//FDwfAnalogInChannelRangeSet(_hdwf, -1, 15);

	// set sample rate and filter
	FDwfAnalogInFrequencySet(_hdwf, getDefaultSampleRate());
	FILTER filter = filterAverage;
	FDwfAnalogInChannelFilterSet(_hdwf, -1, filter);

	// get the maximum buffer size
	//FDwfAnalogInBufferSizeInfo(_hdwf, NULL, &_cBufSamples);
	_cBufSamples = 1024; // max buffer size in open-ephys is 1024?
	FDwfAnalogInBufferSizeSet(_hdwf, _cBufSamples);

	_rgdSamples = new double[_cBufSamples];
}

AnalogDiscov::~AnalogDiscov()
{
	// close the device
	FDwfDeviceClose(_hdwf);
	delete[] _rgdSamples;
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

	// start
	FDwfAnalogInConfigure(_hdwf, 0, true);

	do{
		FDwfAnalogInStatus(_hdwf, true, &_sts);
	} while (_sts != stsDone);

	// get the samples for each channel
	for (int ch = 0; ch < _currentNumChannels; ch++){
		FDwfAnalogInStatusData(_hdwf, ch, _rgdSamples, _cBufSamples);
		// forward the data to the outBuffer
		float* outBuffer = buffer.getWritePointer(ch, 0);
		for (int k = 0; k < _cBufSamples; k++)
			*(outBuffer + k) = (float)_rgdSamples[k];
	}
	timestamp += _cBufSamples;

	setNumSamples(events, _cBufSamples);
}

AudioProcessorEditor* AnalogDiscov::createEditor()
{
    editor = new AnalogDiscovEditor(this);
    return editor;
}

float AnalogDiscov::getDefaultSampleRate()
{
	return 10000.0f;
}

int AnalogDiscov::getNumHeadstageOutputs()
{
	return _currentNumChannels;
}