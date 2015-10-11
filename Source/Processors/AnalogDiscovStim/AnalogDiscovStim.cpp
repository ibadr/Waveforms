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
#include "../Channel/Channel.h"
#include "../Visualization/SpikeObject.h"

#include <stdio.h>


AnalogDiscovStim::AnalogDiscovStim()
    : GenericProcessor("Analog Discov Stim")
{

}

AnalogDiscovStim::~AnalogDiscovStim()
{

}

bool AnalogDiscovStim::enable()
{
    return true;
}

bool AnalogDiscovStim::disable()
{
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

