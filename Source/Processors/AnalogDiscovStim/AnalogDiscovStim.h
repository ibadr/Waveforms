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

#ifndef ANALOGDISCOVSTIM_H_
#define ANALOGDISCOVSTIM_H_

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../GenericProcessor/GenericProcessor.h"

/**

 Takes in MidiEvents and extracts SpikeObjects from the MidiEvent buffers, then sends 
 stimulation pulses to the Analog Discovery output channels based on the incoming Spikes.

  @see GenericProcessor

*/

class AnalogDiscovStim : public GenericProcessor
{
public:

	AnalogDiscovStim();
	~AnalogDiscovStim();

    bool isSink()
    {
        return true;
    }

    void process(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    void handleEvent(int, MidiMessage&, int);

    bool enable();
    bool disable();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogDiscovStim);

};


#endif  // ANALOGDISCOVSTIM_H_
