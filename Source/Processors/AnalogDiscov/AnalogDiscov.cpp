/*
 ------------------------------------------------------------------

 This file is part of the Open Ephys GUI
 Copyright (C) 2015 Islam Badreldin

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
	: GenericProcessor("Analog Discovery"), _timestamp(0), _currentNumChannels(1), _devOpen(false), 
	_fs(10000.0f), _bv(950.57f), _rgdSamples(0)
{

}

AnalogDiscov::~AnalogDiscov()
{
	// close the device
	FDwfDeviceClose(_hdwf); _devOpen = false;
	delete[] _rgdSamples;
}

bool AnalogDiscov::openDevice(int id)
{
	bool isSuccess = false;
	if (id < 0) {
		return isSuccess;
	}
	

	// attempt to open device
	if (_devOpen) // already open under our control
	{
		isSuccess = true;
		return isSuccess;
	}

	BOOL isInUse;
	FDwfEnumDeviceIsOpened(_devId, &isInUse);
	if (isInUse) {
		CoreServices::sendStatusMessage("Device is already in use! Please close other programs that are using the selected device.");
		return isSuccess;
	}
	FDwfDeviceOpen(_devId, &_hdwf);
	_devOpen = true; isSuccess = true;
	return isSuccess;
}

void AnalogDiscov::setDeviceId(int id)
{
	_devId = id; 
		
	int cChannel;
	openDevice(_devId); // attempt to open device

	if (_devOpen) // already open under our control
	{
		// get the number of analog in channels
		FDwfAnalogInChannelCount(_hdwf, &cChannel);
		_currentNumChannels = cChannel;
		// enable channels
		for (int c = 0; c < cChannel; c++){
			FDwfAnalogInChannelEnableSet(_hdwf, c, true);
		}
	}
}

bool AnalogDiscov::enable()
{
	// set sample rate and filter
	FDwfAnalogInFrequencySet(_hdwf, getDefaultSampleRate());
	FILTER filter = filterAverage;
	FDwfAnalogInChannelFilterSet(_hdwf, -1, filter);

	// get the maximum buffer size
	FDwfAnalogInBufferSizeInfo(_hdwf, NULL, &_cBufSamples);
	_cBufSamples = _cBufSamples>1024 ? 1024 : _cBufSamples; // max buffer size in open-ephys is 1024?
	FDwfAnalogInBufferSizeSet(_hdwf, _cBufSamples);

	delete[] _rgdSamples;
	_rgdSamples = new double[_cBufSamples];
    return true;
}

bool AnalogDiscov::disable()
{
	delete[] _rgdSamples; _rgdSamples = 0;
	return true;
}

StringArray AnalogDiscov::devices()
{
	StringArray deviceList;
	int cDevice;
	char deviceName[32];
	FDwfEnum(enumfilterDiscovery, &cDevice);
	for (int i = 0; i < cDevice; i++){
		// we use 0 based indexing
		FDwfEnumDeviceName(i, deviceName);
		deviceList.add(deviceName);
	}
	return deviceList;
}


void AnalogDiscov::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
	setTimestamp(events, _timestamp);

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
			*(outBuffer + k) = (float)_rgdSamples[k]*_bv;
	}
	_timestamp += _cBufSamples;

	setNumSamples(events, _cBufSamples);
}

AudioProcessorEditor* AnalogDiscov::createEditor()
{
    editor = new AnalogDiscovEditor(this);
    return editor;
}

float AnalogDiscov::getDefaultSampleRate()
{
	return _fs;
}

float AnalogDiscov::getDefaultBitVolts()
{
	return _bv;
}

float AnalogDiscov::getBitVolts(Channel* /*chan*/)
{
	return _bv;
}

int AnalogDiscov::getNumHeadstageOutputs()
{
	return _currentNumChannels;
}