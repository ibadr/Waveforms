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
#include "AnalogDiscovManager.h"

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

	void setNumChannels(int n);
	void setDefaultSampleRate(float fs);
	void setDefaultBitVolts(float bv);
	float getDefaultSampleRate();
	float getDefaultBitVolts();
	float getBitVolts(Channel* chan);
	float getBitVoltsInteral(); // access to _bv for the editor
	int getNumHeadstageOutputs();
	//int getNumEventChannels();

	/** Called immediately prior to the start of data acquisition. */
	bool enable();

	/** If false, data acquisition will not start */
	bool isReady();

	/** Called immediately after the end of data acquisition. */
	bool disable();

	/** Returns the list of connected devices */
	StringArray devices();

	void setDeviceId(int id);

    /** Defines the functionality of the processor.
     The process method is called every time a new data buffer is available.
     Adds all the new data that is available to the event data buffer.
     */
    void process(AudioSampleBuffer& buffer, MidiBuffer& events);

    /**
     Returns the Juce GUI component that should be used with this processor.
     */
    AudioProcessorEditor* createEditor();

private:

	int64 _timestamp;
	int _currentNumChannels; // actual number of channels from Analog Discovery Hardware
	int _setNumChannels; // set number of channels, which can be more or less than actual number of channels (extra channels will be filled with zeros)
	int _devId;
	AnalogDiscovManager* _manager;
	bool _devOpen;
	bool _isReady;

	float _fs; // sample rate
	float _bv; // bitVolts (microvolts / digital unit)

	STS _sts;
	double* _rgdSamples;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogDiscov);
};

#endif  // __ANALOGDISCOV_H__
