#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PlaceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PlaceLookAndFeel();

    juce::Font getLabelFont (juce::Label&) override;

    static juce::Colour bgTop() { return juce::Colour (0xff2d1f47); }
    static juce::Colour bgBottom() { return juce::Colour (0xff1a1230); }

    static juce::Colour accent() { return juce::Colour (0xff8b5cf6); }
    static juce::Colour cyan() { return juce::Colour (0xff06b6d4); }

    static juce::Colour textPrimary() { return juce::Colour (0xffe8e0f0); }
    static juce::Colour textSecondary() { return juce::Colour (0x99e8e0f0); }
    static juce::Colour textDim() { return juce::Colour (0x55e8e0f0); }

    static juce::Colour knobTrack() { return juce::Colour (0xff3d2d5a); }
    static juce::Colour knobFill() { return juce::Colour (0xff4a3868); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceLookAndFeel)
};
