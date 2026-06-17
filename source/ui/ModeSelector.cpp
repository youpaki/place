#include "ModeSelector.h"
#include "PlaceLookAndFeel.h"

ModeSelector::ModeSelector()
{
    comboBox.addItem ("STATIC", 1);
    comboBox.addItem ("VOCAL FOLLOW", 2);
    comboBox.setSelectedId (1);
    comboBox.setOpaque (false);
    addAndMakeVisible (comboBox);
}

void ModeSelector::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    juce::Path pillPath;
    pillPath.addRoundedRectangle (bounds, bounds.getHeight() * 0.5f);

    juce::ColourGradient glassGrad (
        juce::Colour (0x18ffffff), bounds.getCentreX(), bounds.getY(),
        juce::Colour (0x08ffffff), bounds.getCentreX(), bounds.getBottom(),
        false);
    g.setGradientFill (glassGrad);
    g.fillPath (pillPath);

    g.setColour (PlaceLookAndFeel::glassBorder());
    g.strokePath (pillPath, juce::PathStrokeType (1.0f));

    int selectedId = comboBox.getSelectedId();
    float halfWidth = bounds.getWidth() * 0.5f;

    auto activeBounds = selectedId == 1
        ? bounds.removeFromLeft (halfWidth)
        : bounds.removeFromRight (halfWidth);

    juce::Path activePath;
    activePath.addRoundedRectangle (activeBounds.reduced (2.0f), activeBounds.getHeight() * 0.45f);

    juce::ColourGradient activeGrad (
        PlaceLookAndFeel::accent().withAlpha (0.3f), activeBounds.getCentreX(), activeBounds.getY(),
        PlaceLookAndFeel::accent().withAlpha (0.15f), activeBounds.getCentreX(), activeBounds.getBottom(),
        false);
    g.setGradientFill (activeGrad);
    g.fillPath (activePath);

    for (int i = 2; i >= 0; --i)
    {
        float glowAlpha = 0.06f * (1.0f - static_cast<float> (i) / 3.0f);
        g.setColour (PlaceLookAndFeel::accent().withAlpha (glowAlpha));
        g.strokePath (activePath, juce::PathStrokeType (1.0f + static_cast<float> (i) * 2.0f));
    }

    g.setFont (juce::FontOptions (10.0f, juce::Font::plain));

    auto leftBounds = bounds.withWidth (halfWidth);
    auto rightBounds = bounds.withX (halfWidth).withWidth (halfWidth);

    if (selectedId == 1)
    {
        leftBounds = activeBounds.reduced (2.0f);
        rightBounds = bounds.removeFromRight (halfWidth);
    }
    else
    {
        rightBounds = activeBounds.reduced (2.0f);
        leftBounds = bounds.removeFromLeft (halfWidth);
    }

    g.setColour (selectedId == 1 ? PlaceLookAndFeel::textPrimary() : PlaceLookAndFeel::textDim());
    g.drawFittedText ("STATIC", leftBounds.toNearestInt(), juce::Justification::centred, 1);

    g.setColour (selectedId == 2 ? PlaceLookAndFeel::textPrimary() : PlaceLookAndFeel::textDim());
    g.drawFittedText ("VOCAL FOLLOW", rightBounds.toNearestInt(), juce::Justification::centred, 1);
}

void ModeSelector::resized()
{
    comboBox.setBounds (getLocalBounds());
    comboBox.setAlpha (0.0f);
}
