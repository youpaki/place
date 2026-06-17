#include "StereoFieldVisualizer.h"
#include "PlaceLookAndFeel.h"

StereoFieldVisualizer::StereoFieldVisualizer()
{
    setInterceptsMouseClicks (false, false);
}

void StereoFieldVisualizer::setSizeAmount (float amount)
{
    if (std::abs (amount - sizeAmount) > 0.01f)
    {
        sizeAmount = amount;
        repaint();
    }
}

void StereoFieldVisualizer::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centre = bounds.getCentre();

    float centerX = centre.x;
    float centerY = centre.y;

    float leftX = centerX - bounds.getWidth() * 0.32f * (1.0f + sizeAmount * 0.25f);
    float rightX = centerX + bounds.getWidth() * 0.32f * (1.0f + sizeAmount * 0.25f);
    float centerWidth = bounds.getWidth() * 0.35f * (1.0f - sizeAmount * 0.45f);
    float sideWidth = bounds.getWidth() * 0.2f;
    float height = bounds.getHeight() * 0.5f;

    g.setColour (PlaceLookAndFeel::accent().withAlpha (0.08f));
    g.fillEllipse (leftX - sideWidth * 0.5f, centerY - height * 0.5f, sideWidth, height);

    g.setColour (PlaceLookAndFeel::accent().withAlpha (0.12f));
    g.fillEllipse (centerX - centerWidth * 0.5f, centerY - height * 0.5f, centerWidth, height);

    g.setColour (PlaceLookAndFeel::accent().withAlpha (0.08f));
    g.fillEllipse (rightX - sideWidth * 0.5f, centerY - height * 0.5f, sideWidth, height);
}
