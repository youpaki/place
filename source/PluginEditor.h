#pragma once

#include "PluginProcessor.h"

namespace Theme {
    const juce::Colour bgMain         = juce::Colour(0xff110a17);
    const juce::Colour bgPanel        = juce::Colour(0xff181122);
    const juce::Colour neonPurple     = juce::Colour(0xffb875ff);
    const juce::Colour dimPurple      = juce::Colour(0xff5a4a75);
    const juce::Colour textLight      = juce::Colour(0xffe6dcf5);
    const juce::Colour knobDark       = juce::Colour(0xff1c1526);
    const juce::Colour panelOutline   = juce::Colour(0xff2a203b);
}

class MainPlaceKnob : public juce::Slider {
public:
    MainPlaceKnob() { setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); }
    void paint(juce::Graphics& g) override;
};

class SideBassKnob : public juce::Slider {
public:
    SideBassKnob() { setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); }
    void paint(juce::Graphics& g) override;
};

class PlaceAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PlaceAudioProcessorEditor (PlaceAudioProcessor&);
    ~PlaceAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PlaceAudioProcessor& audioProcessor;

    MainPlaceKnob mainKnob;
    SideBassKnob sideBassKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mainKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideBassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};
