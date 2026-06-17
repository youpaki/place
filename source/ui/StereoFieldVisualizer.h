#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class StereoFieldVisualizer : public juce::Component, private juce::Timer
{
public:
    StereoFieldVisualizer();
    ~StereoFieldVisualizer() override = default;

    void setSizeAmount (float amount);

    void paint (juce::Graphics& g) override;

private:
    float sizeAmount = 0.0f;
    float smoothedAmount = 0.0f;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoFieldVisualizer)
};
