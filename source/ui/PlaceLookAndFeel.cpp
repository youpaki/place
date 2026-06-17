#include "PlaceLookAndFeel.h"

PlaceLookAndFeel::PlaceLookAndFeel()
{
    setColour (juce::Slider::backgroundColourId, knobTrackColour);
    setColour (juce::Slider::thumbColourId, knobFillColour);
    setColour (juce::Slider::trackColourId, knobFillColour);
    setColour (juce::ComboBox::backgroundColourId, bgColour);
    setColour (juce::ComboBox::outlineColourId, accentColour);
    setColour (juce::ComboBox::textColourId, textColour);
    setColour (juce::ComboBox::arrowColourId, textColour);
    setColour (juce::PopupMenu::backgroundColourId, juce::Colour (0xff2d1b4e));
    setColour (juce::PopupMenu::textColourId, textColour);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, accentColour);
    setColour (juce::Label::textColourId, textColour);
}

void PlaceLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPosProportional, float rotaryStartAngle,
                                         float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (static_cast<float> (x), static_cast<float> (y),
                                          static_cast<float> (width), static_cast<float> (height));

    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.4f;

    g.setColour (knobTrackColour);
    g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

    g.setColour (juce::Colour (0xff3d2b5e));
    g.drawEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    float lineWidth = radius * 0.12f;
    float arcRadius = radius + lineWidth * 0.5f + 2.0f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (centreX, centreY, arcRadius, arcRadius,
                                 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour (knobTrackColour);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved,
                                                        juce::PathStrokeType::rounded));

    float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    juce::Path valueArc;
    valueArc.addCentredArc (centreX, centreY, arcRadius, arcRadius,
                            0.0f, rotaryStartAngle, angle, true);
    g.setColour (knobFillColour);
    g.strokePath (valueArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved,
                                                   juce::PathStrokeType::rounded));

    juce::Path pointer;
    float pointerLength = radius * 0.7f;
    float pointerThickness = 2.5f;
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -radius * 0.85f,
                                 pointerThickness, pointerLength, 1.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (textColour);
    g.fillPath (pointer);

    g.setColour (textColour.withAlpha (0.8f));
    g.setFont (juce::FontOptions (juce::jmin (11.0f, radius * 0.25f)));
    auto textBounds = bounds.removeFromBottom (25.0f);
    g.drawFittedText (slider.getName(), textBounds.toNearestInt(), juce::Justification::centred, 1);
}

void PlaceLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                                     int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                                     juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float> (0.0f, 0.0f,
                                          static_cast<float> (width), static_cast<float> (height));

    g.setColour (juce::Colour (0xff2d1b4e));
    g.fillRoundedRectangle (bounds, 4.0f);

    g.setColour (accentColour.withAlpha (0.6f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 4.0f, 1.0f);

    g.setColour (textColour);
    g.setFont (getComboBoxFont (box));
    g.drawFittedText (box.getText(), bounds.removeFromLeft (bounds.getWidth() - 20).toNearestInt(),
                      juce::Justification::centred, 1);

    auto arrowArea = bounds.removeFromRight (20);
    juce::Path arrow;
    arrow.addTriangle (arrowArea.getCentreX() - 3.0f, arrowArea.getCentreY() - 2.0f,
                       arrowArea.getCentreX() + 3.0f, arrowArea.getCentreY() - 2.0f,
                       arrowArea.getCentreX(), arrowArea.getCentreY() + 3.0f);
    g.setColour (textColour.withAlpha (0.6f));
    g.fillPath (arrow);
}

void PlaceLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    g.fillAll (juce::Colour (0xff2d1b4e));
    g.setColour (accentColour.withAlpha (0.3f));
    g.drawRect (0, 0, width, height, 1);
}

juce::Font PlaceLookAndFeel::getComboBoxFont (juce::ComboBox&)
{
    return juce::FontOptions (13.0f);
}

juce::Font PlaceLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::FontOptions (12.0f);
}
