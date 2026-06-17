#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PlaceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PlaceLookAndFeel();

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override;

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override;

    void drawPopupMenuBackground (juce::Graphics& g, int width, int height) override;

    juce::Font getComboBoxFont (juce::ComboBox&) override;
    juce::Font getLabelFont (juce::Label&) override;

private:
    juce::Colour bgColour { 0xff1a0a2e };
    juce::Colour accentColour { 0xff9b59b6 };
    juce::Colour knobTrackColour { 0xff2d1b4e };
    juce::Colour knobFillColour { 0xff9b59b6 };
    juce::Colour textColour { 0xffecf0f1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceLookAndFeel)
};
