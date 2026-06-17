#include "ModeSelector.h"
#include "PlaceLookAndFeel.h"

ModeSelector::ModeSelector()
{
    setRepaintsOnMouseActivity (true);
    startTimerHz (60);
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

    float halfWidth = bounds.getWidth() * 0.5f;
    float activeX = bounds.getX() + animationPos * halfWidth;
    auto activeBounds = juce::Rectangle<float> (activeX, bounds.getY(), halfWidth, bounds.getHeight());

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
        targetPos = static_cast<float> (selectedMode);

        if (onModeChanged)
            onModeChanged (selectedMode);
    }
}

void ModeSelector::mouseEnter (const juce::MouseEvent& e)
{
    hovered = true;
    repaint();
    Component::mouseEnter (e);
}

void ModeSelector::mouseExit (const juce::MouseEvent& e)
{
    hovered = false;
    repaint();
    Component::mouseExit (e);
}

void ModeSelector::setSelectedMode (int mode)
{
    if (mode != selectedMode && mode >= 0 && mode <= 1)
    {
        selectedMode = mode;
        targetPos = static_cast<float> (selectedMode);
    }
}

void ModeSelector::timerCallback()
{
    float diff = targetPos - animationPos;
    if (std::abs (diff) > 0.01f)
    {
        animationPos += diff * 0.15f;
        repaint();
    }
    else if (animationPos != targetPos)
    {
        animationPos = targetPos;
        repaint();
    }
}
