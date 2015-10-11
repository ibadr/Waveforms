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

#ifndef __ANALOGDISCOVMANAGER_H__
#define __ANALOGDISCOVMANAGER_H__

#ifdef WIN32
#include "../../Resources/windows-libs/WaveFormsSDK/inc/dwf.h"
#else
#include <digilent/waveforms/dwf.h>
#endif

/**

This manager class manages opening/closing of the Analog Discovery hardware using Waveforms SDK

@see AnalogDiscovEditor, AnalogDiscov

*/


class AnalogDiscovManager {
public:
	~AnalogDiscovManager();
	static AnalogDiscovManager * uniqueInst();
	bool registerClientForDevice(int id);
	bool unregisterClientForDevice(int id);
	HDWF currDeviceHdwf() { return _hdwf; }

private:
	/** private constructor, singleton pattern */
	AnalogDiscovManager();

	bool openDevice(int id);
	bool closeDevice(int id);

	static AnalogDiscovManager * _inst; // pointer to the unique instance

	/** FIXME: the following variables need to converted to arrays that scales with the number of available devices */
	HDWF _hdwf; // Digilent Analog Discovery hardware int
	int _numClients; // number of clients using the open device
};

#endif
