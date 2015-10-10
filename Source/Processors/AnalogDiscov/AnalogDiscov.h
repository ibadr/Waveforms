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

#ifndef __ANALOGDISCOV_H__
#define __ANALOGDISCOV_H__

#ifdef WIN32
#include "../../Resources/windows-libs/WaveFormsSDK/inc/dwf.h"
#else
#include <digilent/waveforms/dwf.h>
#endif

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../GenericProcessor/GenericProcessor.h"

#include "AnalogDiscovEditor.h"

/**

 This source processor allows you to record binary data from the Analog Discovery hardware using Waveforms SDK

 @see AnalogDiscovEditor

 */

class AnalogDiscov : public GenericProcessor

{
public:

    /** The class constructor, used to initialize any members. */
    AnalogDiscov();

    /** The class destructor, used to deallocate memory */
    virtual ~AnalogDiscov();

    /**
     Used by the ProcessorGraph to determines whether the processor should be treated as a source.

     Returns always true, since AnalogDiscov is a source.
     */
    bool isSource()
    {
        return true;
    }

	bool generatesTimestamps()
	{
		return true;
	}

	float getDefaultSampleRate();
	int getNumHeadstageOutputs();
	//int getNumEventChannels();

    /**
     This should only be run by the ProcessorGraph, before acquisition will be started.

     It tries to open the serial port previsouly specified by the setDevice and setBaudrate setters.

     Returns true on success, false if port could not be opened.
     */
    bool isReady();

    /**
     Called immediately after the end of data acquisition by the ProcessorGraph.

     It closes the open port serial port.
     */
    bool disable();

    /** Defines the functionality of the processor.

     The process method is called every time a new data buffer is available.

     Adds all the new serial data that is available to the event data buffer.
     */
    void process(AudioSampleBuffer& buffer, MidiBuffer& events);

    /**
     Returns the Juce GUI component that should be used with this processor.
     */
    AudioProcessorEditor* createEditor();

private:

	int64 timestamp;
	int _currentNumChannels;

	HDWF _hdwf;
	STS _sts;
	double* _rgdSamples;
	int _cBufSamples;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogDiscov);
};

#endif  // __ANALOGDISCOV_H__
