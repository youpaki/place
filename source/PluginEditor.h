#pragma once

#include "PluginProcessor.h"
#include "dsp/MidSideProcessor.h"
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
// Custom LookAndFeel to make the ComboBox match your dark neon UI
class PlaceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PlaceLookAndFeel()
    {
        setColour(juce::ComboBox::backgroundColourId, Theme::knobDark);
        setColour(juce::ComboBox::textColourId, Theme::textLight);
        setColour(juce::PopupMenu::backgroundColourId, Theme::bgPanel);
        setColour(juce::PopupMenu::textColourId, Theme::textLight);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, Theme::neonPurple.withAlpha(0.3f));
        setColour(juce::PopupMenu::highlightedTextColourId, Theme::neonPurple);
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override
    {
        auto bounds = box.getLocalBounds().toFloat();
        g.setColour(Theme::bgMain);
        g.fillRoundedRectangle(bounds, 4.0f);
        g.setColour(Theme::panelOutline);
        g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

        juce::Path path;
        auto x = bounds.getRight() - 14.0f;
        auto y = bounds.getCentreY() - 2.0f;
        path.addTriangle(x, y, x + 6.0f, y, x + 3.0f, y + 4.0f);
        g.setColour(Theme::dimPurple);
        g.fillPath(path);
    }
};

//==============================================================================
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

class PillToggle : public juce::ToggleButton {
public:
    PillToggle() {}
    void paintButton(juce::Graphics& g, bool, bool) override;
    void updateAnimation(); 
private:
    float currentPos = 0.0f; 
};

class SegmentedMeter : public juce::Component {
public:
    SegmentedMeter() {}
    void paint(juce::Graphics& g) override;
    void setLevel(float newLevel) { level = newLevel; repaint(); }
private:
    float level = 0.0f; 
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
    PlaceLookAndFeel customLookAndFeel;

    MainPlaceKnob mainKnob;
    SideBassKnob sideBassKnob;
    PillToggle modeToggle;
    
    // Dual Meters
    SegmentedMeter outMeter;
    SegmentedMeter scMeter;
    
    // Sidechain Selector
    juce::ComboBox scSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mainKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideBassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> modeToggleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> scSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};