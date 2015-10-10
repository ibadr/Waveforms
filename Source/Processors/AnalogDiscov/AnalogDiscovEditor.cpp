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

#include "AnalogDiscovEditor.h"
#include "AnalogDiscov.h"

#include <string>
#include <iostream>


AnalogDiscovEditor::AnalogDiscovEditor(AnalogDiscov* parentNode)
    : GenericEditor(parentNode, false)
{
    node = parentNode;

    desiredWidth = 100;

    // Add refresh button
    refreshButton = new UtilityButton("REFRESH", Font("Small Text", 11, Font::bold));
    refreshButton->setRadius(3.0f);
    refreshButton->setBounds(10, 60, 65, 25);
    refreshButton->addListener(this);

    addAndMakeVisible(refreshButton);
}

void AnalogDiscovEditor::startAcquisition()
{
    // Disable the whole gui
    refreshButton->setEnabled(false);
    GenericEditor::startAcquisition();
}

void AnalogDiscovEditor::stopAcquisition()
{
    // Reenable the whole gui
    refreshButton->setEnabled(true);
    GenericEditor::stopAcquisition();
}


void AnalogDiscovEditor::buttonEvent(Button* /*button*/)
{

}

void AnalogDiscovEditor::comboBoxChanged(ComboBox* /*comboBox*/)
{

}

void AnalogDiscovEditor::saveEditorParameters(XmlElement* xmlNode)
{
    XmlElement* parameters = xmlNode->createNewChildElement("PARAMETERS");

}

void AnalogDiscovEditor::loadEditorParameters(XmlElement* xmlNode)
{
    forEachXmlChildElement(*xmlNode, subNode)
    {
        if (subNode->hasTagName("PARAMETERS"))
        {
			;
        }
    }
}

