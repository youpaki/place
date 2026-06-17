#include "PlaceLookAndFeel.h"

PlaceLookAndFeel::PlaceLookAndFeel()
{
    setColour (juce::Label::textColourId, textPrimary());
    setColour (juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    setColour (juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setColour (juce::ComboBox::textColourId, textPrimary());
    setColour (juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
    setColour (juce::Slider::thumbColourId, accent());
    setColour (juce::Slider::trackColourId, accent());
}

juce::Font PlaceLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::FontOptions (13.0f);
}
