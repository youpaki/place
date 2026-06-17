#include "RotaryKnob.h"

RotaryKnob::RotaryKnob (const juce::String& labelText)
{
    slider.setName (labelText);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.2f,
                                juce::MathConstants<float>::pi * 2.8f,
                                true);
    addAndMakeVisible (slider);
}

void RotaryKnob::resized()
{
    slider.setBounds (getLocalBounds());
}
