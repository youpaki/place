#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PremiumKnob : public juce::Component
{
public:
    explicit PremiumKnob (const juce::String& labelText, bool isLarge = false);
    ~PremiumKnob() override = default;

    juce::Slider& getSlider() { return slider; }

    void paint (juce::Graphics& g) override;
    void resized() override;

    void mouseEnter (const juce::MouseEvent&) override;
    void mouseExit (const juce::MouseEvent&) override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

private:
    juce::Slider slider;
    juce::String label;
    bool large;
    bool hovered = false;
    bool pressed = false;
    float pressScale = 1.0f;
    float hoverGlow = 0.0f;

    void drawShadow (juce::Graphics& g, juce::Rectangle<float> bounds) const;
    void drawGlassRing (juce::Graphics& g, juce::Rectangle<float> bounds) const;
    void drawTitaniumDial (juce::Graphics& g, juce::Rectangle<float> bounds, float angle) const;
    void drawReflection (juce::Graphics& g, juce::Rectangle<float> bounds) const;
    void drawLEDPointer (juce::Graphics& g, juce::Rectangle<float> bounds, float angle) const;
    void drawHalo (juce::Graphics& g, juce::Rectangle<float> bounds) const;
    void drawValueArc (juce::Graphics& g, juce::Rectangle<float> bounds, float proportion) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PremiumKnob)
};
