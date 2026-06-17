#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RotaryKnob : public juce::Component
{
public:
    explicit RotaryKnob (const juce::String& labelText);
    ~RotaryKnob() override = default;

    juce::Slider& getSlider() { return slider; }

    void resized() override;

private:
    juce::Slider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
