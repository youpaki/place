#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PlaceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PlaceLookAndFeel();

    juce::Font getLabelFont (juce::Label&) override;

    static juce::Colour bgTop() { return juce::Colour (0xff31204D); }
    static juce::Colour bgMid() { return juce::Colour (0xff24183B); }
    static juce::Colour bgBottom() { return juce::Colour (0xff161022); }

    static juce::Colour accent() { return juce::Colour (0xff9b6dff); }
    static juce::Colour accentGlow() { return juce::Colour (0x409b6dff); }
    static juce::Colour cyanGlow() { return juce::Colour (0xff00e5ff); }

    static juce::Colour textPrimary() { return juce::Colour (0xffe8e0f0); }
    static juce::Colour textSecondary() { return juce::Colour (0x80e8e0f0); }
    static juce::Colour textDim() { return juce::Colour (0x40e8e0f0); }

    static juce::Colour glassBorder() { return juce::Colour (0x18ffffff); }
    static juce::Colour glassFill() { return juce::Colour (0x0affffff); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceLookAndFeel)
};
