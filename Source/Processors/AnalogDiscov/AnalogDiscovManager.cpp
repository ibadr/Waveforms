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

#include "AnalogDiscovManager.h"

#include "../GenericProcessor/GenericProcessor.h"

AnalogDiscovManager* AnalogDiscovManager::_inst = 0;

AnalogDiscovManager::~AnalogDiscovManager()
{

}

AnalogDiscovManager * AnalogDiscovManager::uniqueInst()
{
	if (_inst == 0)
	{
		_inst = new AnalogDiscovManager();
	}
	else
	{
		return _inst;
	}
}

AnalogDiscovManager::AnalogDiscovManager() : _numClients(0), _hdwf(0)
{

}

bool AnalogDiscovManager::openDevice(int id)
{
	bool isSuccess = false;
	if (id < 0) {
		return isSuccess;
	}

	BOOL isInUse;
	FDwfEnumDeviceIsOpened(id, &isInUse);
	if (isInUse) {
		CoreServices::sendStatusMessage("Device is already in use! Please close other programs that are using the selected device.");
		return isSuccess;
	}
	isSuccess=FDwfDeviceOpen(id, &_hdwf);
	return isSuccess;
}

bool AnalogDiscovManager::closeDevice(int /*id*/)
{
	FDwfDeviceClose(_hdwf);
	return true;
}

bool AnalogDiscovManager::registerClientForDevice(int id)
{
	bool isSuccess = false;
	if (_numClients == 0) // first client to request this device
	{
		isSuccess = openDevice(id);
	}
	else
	{
		// device already open under our control for another client
		isSuccess = true;
	}
	if (isSuccess)
		++_numClients;
	else
		CoreServices::sendStatusMessage("Error opening selected device.");
	return isSuccess;
}

bool AnalogDiscovManager::unregisterClientForDevice(int id)
{
	if (--_numClients == 0) // last client to use this device, close
		return closeDevice(id);
	else
		return true;
}
