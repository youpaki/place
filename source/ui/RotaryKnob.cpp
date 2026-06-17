#include "RotaryKnob.h"
#include "PlaceLookAndFeel.h"

PremiumKnob::PremiumKnob (const juce::String& labelText, bool isLarge)
    : label (labelText), large (isLarge)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.2f,
                                juce::MathConstants<float>::pi * 2.8f,
                                true);
    addAndMakeVisible (slider);
    setInterceptsMouseClicks (false, true);
}

void PremiumKnob::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float knobSize = large ? 220.0f : 80.0f;
    float scale = juce::jmin (bounds.getWidth(), bounds.getHeight() - 30.0f) / knobSize;
    float effectiveSize = knobSize * scale * pressScale;

    auto knobArea = bounds.withSizeKeepingCentre (effectiveSize, effectiveSize);

    float proportion = static_cast<float> (slider.valueToProportionOfLength (slider.getValue()));
    float startAngle = juce::MathConstants<float>::pi * 1.2f;
    float endAngle = juce::MathConstants<float>::pi * 2.8f;
    float angle = startAngle + proportion * (endAngle - startAngle);

    drawShadow (g, knobArea);

    if (hovered)
        drawHalo (g, knobArea);

    drawValueArc (g, knobArea, proportion);
    drawGlassRing (g, knobArea);
    drawTitaniumDial (g, knobArea, angle);
    drawReflection (g, knobArea);
    drawLEDPointer (g, knobArea, angle);

    g.setColour (PlaceLookAndFeel::textSecondary());
    g.setFont (juce::FontOptions (large ? 11.0f : 9.0f, juce::Font::plain));
    auto labelArea = bounds.removeFromBottom (25.0f);
    g.drawFittedText (label, labelArea.toNearestInt(), juce::Justification::centred, 1);

    if (large)
    {
        g.setColour (PlaceLookAndFeel::textPrimary());
        g.setFont (juce::FontOptions (16.0f, juce::Font::plain));
        auto valArea = juce::Rectangle<float> (knobArea.getX(), knobArea.getBottom() + 4.0f,
                                                knobArea.getWidth(), 20.0f);
        juce::String valStr = juce::String (static_cast<int> (slider.getValue())) + "%";
        g.drawFittedText (valStr, valArea.toNearestInt(), juce::Justification::centred, 1);
    }
}

void PremiumKnob::resized()
{
    slider.setBounds (getLocalBounds());
}

void PremiumKnob::mouseEnter (const juce::MouseEvent& e)
{
    hovered = true;
    repaint();
    Component::mouseEnter (e);
}

void PremiumKnob::mouseExit (const juce::MouseEvent& e)
{
    hovered = false;
    repaint();
    Component::mouseExit (e);
}

void PremiumKnob::mouseDown (const juce::MouseEvent& e)
{
    pressed = true;
    pressScale = 0.98f;
    repaint();
    Component::mouseDown (e);
}

void PremiumKnob::mouseUp (const juce::MouseEvent& e)
{
    pressed = false;
    pressScale = 1.0f;
    repaint();
    Component::mouseUp (e);
}

void PremiumKnob::drawShadow (juce::Graphics& g, juce::Rectangle<float> bounds) const
{
    auto shadowBounds = bounds.expanded (8.0f).translated (0.0f, 4.0f);
    for (int i = 0; i < 12; ++i)
    {
        float alpha = 0.04f * (1.0f - static_cast<float> (i) / 12.0f);
        g.setColour (juce::Colours::black.withAlpha (alpha));
        g.fillEllipse (shadowBounds.expanded (static_cast<float> (i) * 1.5f));
    }
}

void PremiumKnob::drawGlassRing (juce::Graphics& g, juce::Rectangle<float> bounds) const
{
    auto ringBounds = bounds.expanded (6.0f);

    juce::ColourGradient ringGrad (
        juce::Colour (0x20ffffff), ringBounds.getCentreX(), ringBounds.getY(),
        juce::Colour (0x08ffffff), ringBounds.getCentreX(), ringBounds.getBottom(),
        false);
    g.setGradientFill (ringGrad);
    g.fillEllipse (ringBounds);

    g.setColour (PlaceLookAndFeel::glassBorder());
    g.drawEllipse (ringBounds.getX(), ringBounds.getY(),
                   ringBounds.getWidth(), ringBounds.getHeight(), 1.0f);
}

void PremiumKnob::drawTitaniumDial (juce::Graphics& g, juce::Rectangle<float> bounds, float /*angle*/) const
{
    juce::ColourGradient dialGrad (
        juce::Colour (0xff4a3d6b), bounds.getCentreX(), bounds.getY(),
        juce::Colour (0xff2a1f45), bounds.getCentreX(), bounds.getBottom(),
        false);
    g.setGradientFill (dialGrad);
    g.fillEllipse (bounds);

    auto innerBounds = bounds.reduced (2.0f);
    juce::ColourGradient innerGrad (
        juce::Colour (0xff3d2f5c), innerBounds.getCentreX(), innerBounds.getY(),
        juce::Colour (0xff1e1535), innerBounds.getCentreX(), innerBounds.getBottom(),
        false);
    g.setGradientFill (innerGrad);
    g.fillEllipse (innerBounds);

    g.setColour (juce::Colour (0x10ffffff));
    g.drawEllipse (bounds.getX(), bounds.getY(),
                   bounds.getWidth(), bounds.getHeight(), 0.5f);
}

void PremiumKnob::drawReflection (juce::Graphics& g, juce::Rectangle<float> bounds) const
{
    auto reflBounds = bounds.withHeight (bounds.getHeight() * 0.45f);
    juce::Path reflPath;
    reflPath.addEllipse (reflBounds);

    juce::ColourGradient reflGrad (
        juce::Colour (0x18ffffff), reflBounds.getCentreX(), reflBounds.getY(),
        juce::Colour (0x00ffffff), reflBounds.getCentreX(), reflBounds.getBottom(),
        false);

    g.saveState();
    g.reduceClipRegion (bounds.toNearestInt());
    g.setGradientFill (reflGrad);
    g.fillPath (reflPath);
    g.restoreState();
}

void PremiumKnob::drawLEDPointer (juce::Graphics& g, juce::Rectangle<float> bounds, float angle) const
{
    auto centre = bounds.getCentre();
    float radius = bounds.getWidth() * 0.5f;
    float pointerLength = radius * 0.65f;
    float pointerStart = radius * 0.2f;

    float startX = centre.x + std::sin (angle) * pointerStart;
    float startY = centre.y - std::cos (angle) * pointerStart;
    float endX = centre.x + std::sin (angle) * pointerLength;
    float endY = centre.y - std::cos (angle) * pointerLength;

    for (int i = 4; i >= 0; --i)
    {
        float glowAlpha = 0.08f * (1.0f - static_cast<float> (i) / 5.0f);
        float glowWidth = 2.0f + static_cast<float> (i) * 2.0f;
        g.setColour (PlaceLookAndFeel::accent().withAlpha (glowAlpha));
        g.drawLine (startX, startY, endX, endY, glowWidth);
    }

    g.setColour (PlaceLookAndFeel::accent());
    g.drawLine (startX, startY, endX, endY, 2.0f);

    float dotX = endX;
    float dotY = endY;
    for (int i = 3; i >= 0; --i)
    {
        float glowAlpha = 0.15f * (1.0f - static_cast<float> (i) / 4.0f);
        float dotSize = 3.0f + static_cast<float> (i) * 2.0f;
        g.setColour (PlaceLookAndFeel::accent().withAlpha (glowAlpha));
        g.fillEllipse (dotX - dotSize * 0.5f, dotY - dotSize * 0.5f, dotSize, dotSize);
    }
}

void PremiumKnob::drawHalo (juce::Graphics& g, juce::Rectangle<float> bounds) const
{
    auto haloBounds = bounds.expanded (16.0f);
    for (int i = 0; i < 8; ++i)
    {
        float alpha = 0.03f * (1.0f - static_cast<float> (i) / 8.0f);
        g.setColour (PlaceLookAndFeel::accent().withAlpha (alpha));
        g.fillEllipse (haloBounds.expanded (static_cast<float> (i) * 3.0f));
    }
}

void PremiumKnob::drawValueArc (juce::Graphics& g, juce::Rectangle<float> bounds, float proportion) const
{
    auto arcBounds = bounds.expanded (6.0f);
    float arcRadius = arcBounds.getWidth() * 0.5f;
    float lineWidth = large ? 3.0f : 2.0f;

    float startAngle = juce::MathConstants<float>::pi * 1.2f;
    float endAngle = juce::MathConstants<float>::pi * 2.8f;

    juce::Path bgArc;
    bgArc.addCentredArc (arcBounds.getCentreX(), arcBounds.getCentreY(),
                         arcRadius, arcRadius, 0.0f, startAngle, endAngle, true);
    g.setColour (juce::Colour (0xff1a1230));
    g.strokePath (bgArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));

    if (proportion > 0.001f)
    {
        float valueAngle = startAngle + proportion * (endAngle - startAngle);

        juce::Path valArc;
        valArc.addCentredArc (arcBounds.getCentreX(), arcBounds.getCentreY(),
                              arcRadius, arcRadius, 0.0f, startAngle, valueAngle, true);

        for (int i = 3; i >= 0; --i)
        {
            float glowAlpha = 0.12f * (1.0f - static_cast<float> (i) / 4.0f);
            float glowWidth = lineWidth + static_cast<float> (i) * 2.0f;
            g.setColour (PlaceLookAndFeel::accent().withAlpha (glowAlpha));
            g.strokePath (valArc, juce::PathStrokeType (glowWidth, juce::PathStrokeType::curved,
                                                         juce::PathStrokeType::rounded));
        }

        g.setColour (PlaceLookAndFeel::accent());
        g.strokePath (valArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved,
                                                     juce::PathStrokeType::rounded));
    }
}
