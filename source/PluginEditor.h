#pragma once

#include "PluginProcessor.h"

//==============================================================================
namespace Theme {
    const juce::Colour bgMain         = juce::Colour(0xff110a17);
    const juce::Colour bgPanel        = juce::Colour(0xff181122);
    const juce::Colour neonPurple     = juce::Colour(0xffb875ff);
    const juce::Colour dimPurple      = juce::Colour(0xff5a4a75);
    const juce::Colour textLight      = juce::Colour(0xffe6dcf5);
    const juce::Colour knobDark       = juce::Colour(0xff1c1526);
    const juce::Colour panelOutline   = juce::Colour(0xff2a203b);
}

//==============================================================================
class MainPlaceKnob : public juce::Slider
{
public:
    MainPlaceKnob() { setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); }
    void paint(juce::Graphics& g) override;
};

class SideBassKnob : public juce::Slider
{
public:
    SideBassKnob() { setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); }
    void paint(juce::Graphics& g) override;
};

class PillToggle : public juce::ToggleButton
{
public:
    PillToggle() {}
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    // Call this inside the editor's timer to animate the thumb
    void updateAnimation(); 

private:
    float currentPos = 0.0f; // Stores the animated position of the thumb
};

class SegmentedMeter : public juce::Component
{
public:
    SegmentedMeter() {}
    void paint(juce::Graphics& g) override;
    void setLevel(float newLevel) { level = newLevel; repaint(); }
private:
    float level = 0.0f; 
};

class SCButton : public juce::ToggleButton
{
public:
    SCButton() {}
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

//==============================================================================
class PlaceAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    PlaceAudioProcessorEditor (PlaceAudioProcessor&);
    ~PlaceAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    PlaceAudioProcessor& audioProcessor;

    MainPlaceKnob mainKnob;
    SideBassKnob sideBassKnob;
    PillToggle modeToggle;
    SegmentedMeter ledMeter;
    SCButton scButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mainKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideBassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> modeToggleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};