#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PremiumKnob : public juce::Component
{
public:
    explicit PremiumKnob (const juce::String& labelText, bool isLarge = false);
    ~PremiumKnob() override = default;

    juce::Slider& getSlider() { return slider; }

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Slider slider;
    juce::String label;
    bool large;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PremiumKnob)
};
