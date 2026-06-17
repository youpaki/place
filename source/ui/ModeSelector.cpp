#include "ModeSelector.h"
#include "PlaceLookAndFeel.h"

ModeSelector::ModeSelector()
{
    setRepaintsOnMouseActivity (true);
}

void ModeSelector::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (0.5f);

    juce::Path pillPath;
    pillPath.addRoundedRectangle (bounds, bounds.getHeight() * 0.5f);

    g.setColour (PlaceLookAndFeel::knobTrack());
    g.fillPath (pillPath);

    g.setColour (juce::Colour (0x20ffffff));
    g.strokePath (pillPath, juce::PathStrokeType (1.0f));

    float halfWidth = bounds.getWidth() * 0.5f;
    auto activeBounds = selectedMode == 0
        ? juce::Rectangle<float> (bounds.getX(), bounds.getY(), halfWidth, bounds.getHeight())
        : juce::Rectangle<float> (bounds.getX() + halfWidth, bounds.getY(), halfWidth, bounds.getHeight());

    juce::Path activePath;
    activePath.addRoundedRectangle (activeBounds.reduced (2.0f), activeBounds.getHeight() * 0.45f);

    g.setColour (PlaceLookAndFeel::accent().withAlpha (0.25f));
    g.fillPath (activePath);

    g.setFont (juce::FontOptions (10.0f));

    auto leftTextBounds = juce::Rectangle<float> (bounds.getX(), bounds.getY(), halfWidth, bounds.getHeight());
    auto rightTextBounds = juce::Rectangle<float> (bounds.getX() + halfWidth, bounds.getY(), halfWidth, bounds.getHeight());

    g.setColour (selectedMode == 0 ? PlaceLookAndFeel::textPrimary() : PlaceLookAndFeel::textDim());
    g.drawFittedText ("STATIC", leftTextBounds.toNearestInt(), juce::Justification::centred, 1);

    g.setColour (selectedMode == 1 ? PlaceLookAndFeel::textPrimary() : PlaceLookAndFeel::textDim());
    g.drawFittedText ("VOCAL FOLLOW", rightTextBounds.toNearestInt(), juce::Justification::centred, 1);
}

void ModeSelector::resized()
{
}

void ModeSelector::mouseDown (const juce::MouseEvent& e)
{
    auto bounds = getLocalBounds().toFloat();
    float halfWidth = bounds.getWidth() * 0.5f;

    int newMode = (e.x < halfWidth) ? 0 : 1;

    if (newMode != selectedMode)
    {
        selectedMode = newMode;
        repaint();

        if (onModeChanged)
            onModeChanged (selectedMode);
    }
}

void ModeSelector::setSelectedMode (int mode)
{
    if (mode != selectedMode && mode >= 0 && mode <= 1)
    {
        selectedMode = mode;
        repaint();
    }
}
