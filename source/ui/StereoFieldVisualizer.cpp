#include "StereoFieldVisualizer.h"
#include "PlaceLookAndFeel.h"

StereoFieldVisualizer::StereoFieldVisualizer()
{
    setInterceptsMouseClicks (false, false);
    startTimerHz (60);
}

void StereoFieldVisualizer::setSizeAmount (float amount)
{
    sizeAmount = amount;
}

void StereoFieldVisualizer::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centre = bounds.getCentre();

    float centerX = centre.x;
    float centerY = centre.y;

    float leftX = centerX - bounds.getWidth() * 0.35f * (1.0f + smoothedAmount * 0.3f);
    float rightX = centerX + bounds.getWidth() * 0.35f * (1.0f + smoothedAmount * 0.3f);

    float centerWidth = bounds.getWidth() * 0.4f * (1.0f - smoothedAmount * 0.5f);
    float sideWidth = bounds.getWidth() * 0.25f * (1.0f + smoothedAmount * 0.2f);
    float height = bounds.getHeight() * 0.6f;

    for (int i = 5; i >= 0; --i)
    {
        float alpha = 0.04f * (1.0f - static_cast<float> (i) / 6.0f);
        float expand = static_cast<float> (i) * 8.0f;

        g.setColour (PlaceLookAndFeel::accent().withAlpha (alpha * 0.6f));
        g.fillEllipse (leftX - sideWidth * 0.5f - expand,
                       centerY - height * 0.5f - expand,
                       sideWidth + expand * 2.0f,
                       height + expand * 2.0f);

        g.setColour (PlaceLookAndFeel::accent().withAlpha (alpha));
        g.fillEllipse (centerX - centerWidth * 0.5f - expand,
                       centerY - height * 0.5f - expand,
                       centerWidth + expand * 2.0f,
                       height + expand * 2.0f);

        g.setColour (PlaceLookAndFeel::accent().withAlpha (alpha * 0.6f));
        g.fillEllipse (rightX - sideWidth * 0.5f - expand,
                       centerY - height * 0.5f - expand,
                       sideWidth + expand * 2.0f,
                       height + expand * 2.0f);
    }
}

void StereoFieldVisualizer::timerCallback()
{
    float diff = sizeAmount - smoothedAmount;
    if (std::abs (diff) > 0.001f)
    {
        smoothedAmount += diff * 0.08f;
        repaint();
    }
}
