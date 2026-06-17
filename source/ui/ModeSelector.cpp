#include "ModeSelector.h"

ModeSelector::ModeSelector()
{
    comboBox.addItem ("Static", 1);
    comboBox.addItem ("Vocal Follow", 2);
    comboBox.setSelectedId (1);
    addAndMakeVisible (comboBox);
}

void ModeSelector::resized()
{
    comboBox.setBounds (getLocalBounds());
}
