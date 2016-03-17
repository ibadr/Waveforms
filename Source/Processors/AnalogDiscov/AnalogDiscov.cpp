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
	: GenericProcessor("Analog Discovery"), _timestamp(0), _currentNumChannels(1), _setNumChannels(1), _devOpen(false), _isReady(false),
	_fs(10000.0f), _bv(950.57f), _rgdSamples(0)
{
	_manager = AnalogDiscovManager::uniqueInst();
	// attempt to automatically open the first device, if available
	setDeviceId(0);
}

AnalogDiscov::~AnalogDiscov()
{
	// close the device
	_manager->unregisterClientForDevice(_devId);
	delete[] _rgdSamples;
}

void AnalogDiscov::setDeviceId(int id)
{
	if (_devId != id)
	{
		// unregister the older device first
		_manager->unregisterClientForDevice(id);
	}
	_devId = id; 

	if (!_devOpen)
		_devOpen=_manager->registerClientForDevice(_devId); // open the device and register a new client

	if (_devOpen) // already open under our control
	{
		// get the number of analog in channels
		int cChannel;
		FDwfAnalogInChannelCount(_manager->currDeviceHdwf(), &cChannel);
		_currentNumChannels = cChannel;
		_setNumChannels = _currentNumChannels;
		// enable channels
		for (int c = 0; c < cChannel; c++){
			FDwfAnalogInChannelEnableSet(_manager->currDeviceHdwf(), c, true);
		}
	}
}

bool AnalogDiscov::enable()
{
	HDWF hdwf = _manager->currDeviceHdwf();
	_isReady = false;
	if (hdwf > 0) {
		BOOL isSuccess = 0;
		// set sample rate and filter
		isSuccess=FDwfAnalogInFrequencySet(hdwf, getDefaultSampleRate());
		FILTER filter = filterAverage;
		isSuccess=FDwfAnalogInChannelFilterSet(hdwf, -1, filter);

		// get the maximum buffer size
		isSuccess=FDwfAnalogInBufferSizeInfo(hdwf, NULL, &_cBufSamples);
		_cBufSamples = _cBufSamples > 1024 ? 1024 : _cBufSamples; // max buffer size in open-ephys is 1024?
		isSuccess=FDwfAnalogInBufferSizeSet(hdwf, _cBufSamples);

		delete[] _rgdSamples;
		_rgdSamples = new double[_cBufSamples];

		_isReady = isSuccess > 0 ? true : false;
		return _isReady;
	}
	else { return false; }
}

bool AnalogDiscov::isReady()
{
	return enable();
}

bool AnalogDiscov::disable()
{
	delete[] _rgdSamples; _rgdSamples = 0;
	_isReady = false;
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

	HDWF hdwf = _manager->currDeviceHdwf();
	// start
	FDwfAnalogInConfigure(hdwf, 0, true);

	do{
		FDwfAnalogInStatus(hdwf, true, &_sts);
	} while (_sts != stsDone);

	// get the samples for each channel
	for (int ch = 0; ch < _currentNumChannels; ch++){
		FDwfAnalogInStatusData(hdwf, ch, _rgdSamples, _cBufSamples);
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

void AnalogDiscov::setNumChannels(int n)
{
	_setNumChannels = n;
}

void AnalogDiscov::setDefaultSampleRate(float fs)
{
	_fs = fs;
}

void AnalogDiscov::setDefaultBitVolts(float bv)
{
	_bv = bv;
}

float AnalogDiscov::getDefaultSampleRate()
{
	return _fs;
}

float AnalogDiscov::getDefaultBitVolts()
{
	return 1.0; // since data is saved in Int16 format, it's better to store the uV values as they are
}

float AnalogDiscov::getBitVolts(Channel* /*chan*/)
{
	return 1.0; // since data is saved in Int16 format, it's better to store the uV values as they are
}

float AnalogDiscov::getBitVoltsInteral() {
	return _bv;
}

int AnalogDiscov::getNumHeadstageOutputs()
{
	return _setNumChannels;
}