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
	ret=FDwfAnalogOutNodeFunctionSet(hdwf, 0, AnalogOutNodeCarrier, funcSine);
	// 80 Hz
	ret=FDwfAnalogOutNodeFrequencySet(hdwf, 0, AnalogOutNodeCarrier, 80.0);
	// 1.41V amplitude (1Vrms), 2.82V pk2pk
	ret=FDwfAnalogOutNodeAmplitudeSet(hdwf, 0, AnalogOutNodeCarrier, 1.41);
	// start signal generation
	ret=FDwfAnalogOutConfigure(hdwf, 0, true);
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
				; // do something
            }
        }
    }
}

