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
    _node = parentNode;

    desiredWidth = 175;

	initEditor();
	buttonEvent(_refreshButton); // refresh after construction
}

void AnalogDiscovEditor::initEditor()
{
	// Add device list
	_deviceList = new ComboBox();
	_deviceList->setBounds(10, 30, 150, 25);
	_deviceList->addListener(this);
	addAndMakeVisible(_deviceList);

	// Add refresh button
	_refreshButton = new UtilityButton("REFRESH", Font("Small Text", 10, Font::plain));
	_refreshButton->setRadius(3.0f);
	_refreshButton->setBounds(85, 60, 65, 20);
	_refreshButton->addListener(this);
	addAndMakeVisible(_refreshButton);

	_sampleRateLabel = new Label("sample rate label", "Sample rate:");
	_sampleRateLabel->setBounds(10, 85, 80, 20);
	_sampleRateLabel->setFont(Font("Small Text", 11, Font::plain));
	_sampleRateLabel->setColour(Label::textColourId, Colours::darkgrey);
	addAndMakeVisible(_sampleRateLabel);

	_uVperUnitLabel = new Label("uV/unit label", "uV/unit:");
	_uVperUnitLabel->setBounds(10, 110, 80, 20);
	_uVperUnitLabel->setFont(Font("Small Text", 11, Font::plain));
	_uVperUnitLabel->setColour(Label::textColourId, Colours::darkgrey);
	addAndMakeVisible(_uVperUnitLabel);

	_sampleRateValue = new Label("sample rate value", String(_node->getDefaultSampleRate()));
	_sampleRateValue->setBounds(100, 85, 50, 20);
	_sampleRateValue->setFont(Font("Default", 11, Font::plain));
	_sampleRateValue->setColour(Label::textColourId, Colours::white);
	_sampleRateValue->setColour(Label::backgroundColourId, Colours::grey);
	_sampleRateValue->setEditable(true);
	_sampleRateValue->addListener(this);
	_sampleRateValue->setTooltip("Set the sample rate for the selected channels");
	addAndMakeVisible(_sampleRateValue);

	_uVperUnitValue = new Label("uV/unit label", String(_node->getDefaultBitVolts()));
	_uVperUnitValue->setBounds(100, 110, 50, 20);
	_uVperUnitValue->setFont(Font("Default", 11, Font::plain));
	_uVperUnitValue->setColour(Label::textColourId, Colours::white);
	_uVperUnitValue->setColour(Label::backgroundColourId, Colours::grey);
	_uVperUnitValue->setEditable(true);
	_uVperUnitValue->addListener(this);
	_uVperUnitValue->setTooltip("Set the uV/digital unit scaling for the selected channels");
	addAndMakeVisible(_uVperUnitValue);
}

void AnalogDiscovEditor::startAcquisition()
{
    // Disable the whole gui
	_deviceList->setEnabled(false);
	_refreshButton->setEnabled(false);
	_uVperUnitValue->setEnabled(false);
	_sampleRateValue->setEnabled(false);
    GenericEditor::startAcquisition();
}

void AnalogDiscovEditor::stopAcquisition()
{
    // Reenable the whole gui
	_deviceList->setEnabled(true);
    _refreshButton->setEnabled(true);
	_uVperUnitValue->setEnabled(true);
	_sampleRateValue->setEnabled(true);
    GenericEditor::stopAcquisition();
}


void AnalogDiscovEditor::buttonEvent(Button* /*button*/)
{
	_deviceList->clear(); 
	StringArray dev=_node->devices();
	_deviceList->addItemList(dev, 1);
	if (dev.size() > 0) {
		_deviceList->setSelectedId(1);
	}
}

void AnalogDiscovEditor::comboBoxChanged(ComboBox* comboBox)
{
	_node->setDeviceId(comboBox->getSelectedId()-1);
}

void AnalogDiscovEditor::labelTextChanged(Label* label)
{
	if (label == _uVperUnitValue)
	{
		_node->setDefaultBitVolts(label->getText().getFloatValue());
	}
	else if (label == _sampleRateValue)
	{
		float v = label->getText().getFloatValue();
		if (v < 1024) {
			CoreServices::sendStatusMessage("Please set the sample rate to at least 1024 Hz!");
			label->setText(String(_node->getDefaultSampleRate()), NotificationType::dontSendNotification);
			return;
		}
		_node->setDefaultSampleRate(v);
	}
	return;
}

void AnalogDiscovEditor::saveCustomParameters(XmlElement* xmlNode)
{
	xmlNode->setAttribute("Type", "AnalogDiscovEditor");

    XmlElement* parameters = xmlNode->createNewChildElement("PARAMETERS");
	parameters->setAttribute("SampleRate", _sampleRateValue->getText());
	parameters->setAttribute("BitVolts", _uVperUnitValue->getText());
}

void AnalogDiscovEditor::loadCustomParameters(XmlElement* xmlNode)
{
    forEachXmlChildElement(*xmlNode, subNode)
    {
        if (subNode->hasTagName("PARAMETERS"))
        {
			_sampleRateValue->setText(subNode->getStringAttribute("SampleRate", "10000"), NotificationType::sendNotificationAsync);
			_uVperUnitValue->setText(subNode->getStringAttribute("BitVolts", "1000"), NotificationType::sendNotificationAsync);
        }
    }
}

