#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ModeSelector : public juce::Component
{
public:
    ModeSelector();
    ~ModeSelector() override = default;

    juce::ComboBox& getComboBox() { return comboBox; }

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::ComboBox comboBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeSelector)
};
