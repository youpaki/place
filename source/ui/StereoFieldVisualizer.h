#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class StereoFieldVisualizer : public juce::Component
{
public:
    StereoFieldVisualizer();
    ~StereoFieldVisualizer() override = default;

    void setSizeAmount (float amount);
    void paint (juce::Graphics& g) override;

private:
    float sizeAmount = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoFieldVisualizer)
};
