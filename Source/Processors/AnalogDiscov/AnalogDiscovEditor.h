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

#ifndef __ANALOGDISCOVEDITOR_H__
#define __ANALOGDISCOVEDITOR_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Editors/GenericEditor.h"

class AnalogDiscov;

class AnalogDiscovEditor : public GenericEditor, public ComboBox::Listener
{

public:

    AnalogDiscovEditor(AnalogDiscov* parentNode);

    /** Button listener callback, called by button when pressed. */
    void buttonEvent(Button* button);

    /** Combobox listener callback, callewd when a combobox is changed. */
    void comboBoxChanged(ComboBox* box);

    /** Called by processor graph in beginning of the acqusition, disables editor completly. */
    void startAcquisition();

    /** Called by processor graph at the end of the acqusition, reenables editor completly. */
    void stopAcquisition();

    /** Called when configuration is saved. Adds editors config to xml. */
    void saveEditorParameters(XmlElement* xml);

    /** Called when configuration is loaded. Reads editors config from xml. */
    void loadEditorParameters(XmlElement* xml);

private:

    // Button that reloads device list
    ScopedPointer<UtilityButton> refreshButton;

    // Parent node
    AnalogDiscov* node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogDiscovEditor);
};


#endif  // __ANALOGDISCOVEDITOR_H__