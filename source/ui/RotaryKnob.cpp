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
}

void PremiumKnob::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float knobSize = large ? 160.0f : 70.0f;
    float scale = juce::jmin (bounds.getWidth(), bounds.getHeight() - 25.0f) / knobSize;
    float effectiveSize = knobSize * scale;

    auto knobArea = bounds.withSizeKeepingCentre (effectiveSize, effectiveSize);
    auto centre = knobArea.getCentre();
    float radius = effectiveSize * 0.5f;

    float proportion = static_cast<float> (slider.valueToProportionOfLength (slider.getValue()));
    float startAngle = juce::MathConstants<float>::pi * 1.2f;
    float endAngle = juce::MathConstants<float>::pi * 2.8f;
    float angle = startAngle + proportion * (endAngle - startAngle);

    g.setColour (PlaceLookAndFeel::knobFill());
    g.fillEllipse (knobArea);

    g.setColour (PlaceLookAndFeel::knobTrack());
    g.drawEllipse (knobArea.getX(), knobArea.getY(),
                   knobArea.getWidth(), knobArea.getHeight(), 1.5f);

    float arcRadius = radius + 4.0f;
    float lineWidth = large ? 2.5f : 2.0f;

    juce::Path bgArc;
    bgArc.addCentredArc (centre.x, centre.y, arcRadius, arcRadius,
                         0.0f, startAngle, endAngle, true);
    g.setColour (PlaceLookAndFeel::knobTrack());
    g.strokePath (bgArc, juce::PathStrokeType (lineWidth));

    if (proportion > 0.001f)
    {
        float valueAngle = startAngle + proportion * (endAngle - startAngle);
        juce::Path valArc;
        valArc.addCentredArc (centre.x, centre.y, arcRadius, arcRadius,
                              0.0f, startAngle, valueAngle, true);
        g.setColour (PlaceLookAndFeel::accent());
        g.strokePath (valArc, juce::PathStrokeType (lineWidth));
    }

    float pointerLength = radius * 0.6f;
    float pointerStart = radius * 0.15f;
    float startX = centre.x + std::sin (angle) * pointerStart;
    float startY = centre.y - std::cos (angle) * pointerStart;
    float endX = centre.x + std::sin (angle) * pointerLength;
    float endY = centre.y - std::cos (angle) * pointerLength;

    g.setColour (PlaceLookAndFeel::textPrimary());
    g.drawLine (startX, startY, endX, endY, 2.0f);

    g.setColour (PlaceLookAndFeel::textSecondary());
    g.setFont (juce::FontOptions (large ? 10.0f : 9.0f));
    auto labelArea = bounds.removeFromBottom (20.0f);
    g.drawFittedText (label, labelArea.toNearestInt(), juce::Justification::centred, 1);

    if (large)
    {
        g.setColour (PlaceLookAndFeel::textPrimary());
        g.setFont (juce::FontOptions (14.0f));
        juce::String valStr = juce::String (static_cast<int> (slider.getValue())) + "%";
        auto valArea = juce::Rectangle<float> (knobArea.getX(), knobArea.getBottom() + 2.0f,
                                                knobArea.getWidth(), 18.0f);
        g.drawFittedText (valStr, valArea.toNearestInt(), juce::Justification::centred, 1);
    }
}

void PremiumKnob::resized()
{
    slider.setBounds (getLocalBounds());
}
