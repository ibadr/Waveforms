/*
 ------------------------------------------------------------------

 This file is part of the Open Ephys GUI
 Copyright (C) 2014 Florian Franzen

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

#include "AnalogDiscovEditor.h"
#include "AnalogDiscov.h"

#include <string>
#include <iostream>


AnalogDiscovEditor::AnalogDiscovEditor(AnalogDiscov* parentNode)
    : GenericEditor(parentNode, false)
{
    node = parentNode;

    desiredWidth = 180;

    // Add device list
    deviceList = new ComboBox();
    deviceList->setBounds(10,30,150,25);
    deviceList->addListener(this);
	deviceList->addItemList(StringArray("D1"), 1);

    addAndMakeVisible(deviceList);


    // Add baudrate list
    baudrateList = new ComboBox();
    baudrateList->setBounds(10,60,80,25);
    baudrateList->addListener(this);

	int tmp[3] = { 1, 2, 3 };
	Array<int> baudrates = Array<int>(tmp);

    for (int i = 0; i < baudrates.size(); i++)
    {
        baudrateList->addItem(String(i+1), i+1);
    }

    addAndMakeVisible(baudrateList);

    // Add refresh button
    refreshButton = new UtilityButton("REFRESH", Font("Small Text", 13, Font::bold));
    refreshButton->setRadius(3.0f);
    refreshButton->setBounds(95, 60, 65, 25);
    refreshButton->addListener(this);

    addAndMakeVisible(refreshButton);
}

void AnalogDiscovEditor::startAcquisition()
{
    // Disable the whole gui
    deviceList->setEnabled(false);
    baudrateList->setEnabled(false);
    refreshButton->setEnabled(false);
    GenericEditor::startAcquisition();
}

void AnalogDiscovEditor::stopAcquisition()
{
    // Reenable the whole gui
    deviceList->setEnabled(true);
    baudrateList->setEnabled(true);
    refreshButton->setEnabled(true);
    GenericEditor::stopAcquisition();
}


void AnalogDiscovEditor::buttonEvent(Button* button)
{
    // Refresh list of devices
    deviceList->clear();
	deviceList->addItemList(StringArray("D1"), 1);
}

void AnalogDiscovEditor::comboBoxChanged(ComboBox* comboBox)
{/*
    // Push new selection to parent node
    if (comboBox == deviceList)
    {
        node->setDevice("D1");
    }
    else if (comboBox == baudrateList)
    {
        node->setBaudrate(0);
    }
	*/
}

void AnalogDiscovEditor::saveEditorParameters(XmlElement* xmlNode)
{
    XmlElement* parameters = xmlNode->createNewChildElement("PARAMETERS");

    parameters->setAttribute("device", deviceList->getText().toStdString());
    parameters->setAttribute("baudrate", baudrateList->getSelectedId());
}

void AnalogDiscovEditor::loadEditorParameters(XmlElement* xmlNode)
{
    forEachXmlChildElement(*xmlNode, subNode)
    {
        if (subNode->hasTagName("PARAMETERS"))
        {
            deviceList->setText(subNode->getStringAttribute("device", ""));
            baudrateList->setSelectedId(subNode->getIntAttribute("baudrate"));
        }
    }
}

