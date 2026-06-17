#include "PlaceLookAndFeel.h"

PlaceLookAndFeel::PlaceLookAndFeel()
{
    setColour (juce::Label::textColourId, textPrimary());
    setColour (juce::Slider::backgroundColourId, knobTrack());
    setColour (juce::Slider::thumbColourId, accent());
    setColour (juce::Slider::trackColourId, accent());
}

juce::Font PlaceLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::FontOptions (13.0f);
}
