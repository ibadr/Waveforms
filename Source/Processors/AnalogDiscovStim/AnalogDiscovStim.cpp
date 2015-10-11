/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

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

#include "AnalogDiscovStim.h"
#include "../AnalogDiscov/AnalogDiscovManager.h"
#include "../Visualization/SpikeObject.h"

#include <stdio.h>


AnalogDiscovStim::AnalogDiscovStim()
	: GenericProcessor("Analog Discov Stim"), _isReady(false)
{
	_manager = AnalogDiscovManager::uniqueInst();
	_isReady=_manager->registerClientForDevice(0);
}

AnalogDiscovStim::~AnalogDiscovStim()
{
	_manager->unregisterClientForDevice(0);
}

bool AnalogDiscovStim::isReady()
{
	HDWF hdwf = _manager->currDeviceHdwf();
	BOOL ret;
	ret=FDwfAnalogOutNodeEnableSet(hdwf, 0, AnalogOutNodeCarrier, true);
	// set sine function
	ret = FDwfAnalogOutNodeFunctionSet(hdwf, 0, AnalogOutNodeCarrier, funcSquare);
	// 1 kHz
	ret=FDwfAnalogOutNodeFrequencySet(hdwf, 0, AnalogOutNodeCarrier, 1000.0);
	// 0.5 V amplitude, 1 V pk2pk
	ret=FDwfAnalogOutNodeAmplitudeSet(hdwf, 0, AnalogOutNodeCarrier, 0.5);
	// run for only 5 ms each time
	ret = FDwfAnalogOutRunSet(hdwf, 0, 5*1e-3);
	return _isReady && ret>0? true:false;
}


bool AnalogDiscovStim::enable()
{
    return true;
}

bool AnalogDiscovStim::disable()
{
	HDWF hdwf = _manager->currDeviceHdwf();
	FDwfAnalogOutConfigure(hdwf, 0, false);
    return true;
}

void AnalogDiscovStim::process(AudioSampleBuffer& /*buffer*/, MidiBuffer& events)
{
    checkForEvents(events); // automatically calls handleEvent
	// we're not interested in processing the continuous data in the 'buffer', only the spike events
}

void AnalogDiscovStim::handleEvent(int eventType, MidiMessage& event, int /*sampleNum*/)
{

    if (eventType == SPIKE)
    {

        const uint8_t* dataptr = event.getRawData();
        int bufferSize = event.getRawDataSize();

        if (bufferSize > 0)
        {

            SpikeObject newSpike;

            bool isValid = unpackSpike(&newSpike, dataptr, bufferSize);

            if (isValid)
            {
				if (newSpike.channel == 0 && newSpike.sortedId == 1)
				{
					// run stim once
					FDwfAnalogOutConfigure(_manager->currDeviceHdwf(), 0, true);
				}
            }
        }
    }
}

