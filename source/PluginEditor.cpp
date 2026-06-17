#include "PluginEditor.h"

PlaceAudioProcessorEditor::PlaceAudioProcessorEditor (PlaceAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p),
      sizeMaximizerKnob ("SIZE MAXIMIZER", true),
      sideBassRemoverKnob ("SIDE BASS REMOVER", false)
{
    setLookAndFeel (&lookAndFeel);
    setResizable (true, true);
    setResizeLimits (480, 270, 1280, 720);

    addAndMakeVisible (stereoField);
    addAndMakeVisible (sizeMaximizerKnob);
    addAndMakeVisible (sideBassRemoverKnob);
    addAndMakeVisible (modeSelector);

    scIndicator.setText ("SC OFF", juce::dontSendNotification);
    scIndicator.setJustificationType (juce::Justification::centred);
    scIndicator.setFont (juce::FontOptions (10.0f));
    scIndicator.setColour (juce::Label::textColourId, PlaceLookAndFeel::textDim());
    addAndMakeVisible (scIndicator);

    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIZE_MAXIMIZER, sizeMaximizerKnob.getSlider());

    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIDE_BASS_REMOVER, sideBassRemoverKnob.getSlider());

    modeSelector.onModeChanged = [this](int mode)
    {
        if (auto* param = processorRef.getAPVTS().getParameter (ParamIDs::MODE))
            param->setValueNotifyingHost (static_cast<float> (mode));
    };

    generateNoiseTexture();
    setSize (defaultWidth, defaultHeight);
    startTimerHz (60);
}

PlaceAudioProcessorEditor::~PlaceAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void PlaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    paintBackground (g);
    paintBloom (g);

    g.setColour (PlaceLookAndFeel::textPrimary());
    g.setFont (juce::FontOptions (24.0f, juce::Font::plain));
    g.drawFittedText ("P L A C E", getLocalBounds().removeFromTop (45), juce::Justification::centred, 1);

    g.setColour (PlaceLookAndFeel::textSecondary());
    g.setFont (juce::FontOptions (11.0f, juce::Font::plain));
    g.drawFittedText ("by leve", getLocalBounds().removeFromTop (25).removeFromTop (15),
                      juce::Justification::centred, 1);

    paintSCIndicator (g);

    g.setColour (PlaceLookAndFeel::textDim());
    g.setFont (juce::FontOptions (9.0f, juce::Font::plain));
    g.drawFittedText ("v1.0", getLocalBounds().removeFromBottom (18).removeFromRight (50),
                      juce::Justification::centredRight, 1);

    paintVignette (g);
    paintGlassBorder (g);
}

void PlaceAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (60);

    int knobSize = juce::jmin (area.getWidth() / 2, area.getHeight() / 2);
    knobSize = juce::jmax (knobSize, 120);

    auto centerArea = area.withSizeKeepingCentre (knobSize, knobSize + 40);
    stereoField.setBounds (centerArea.expanded (40));
    sizeMaximizerKnob.setBounds (centerArea);

    area.removeFromTop (centerArea.getHeight());

    int smallKnobSize = juce::jmax (knobSize / 3, 60);
    int bottomHeight = juce::jmax (smallKnobSize + 30, 80);
    auto bottomArea = area.removeFromBottom (bottomHeight);

    int thirdWidth = bottomArea.getWidth() / 3;

    auto bassArea = bottomArea.removeFromLeft (thirdWidth);
    sideBassRemoverKnob.setBounds (bassArea.withSizeKeepingCentre (smallKnobSize, smallKnobSize + 25));

    auto modeArea = bottomArea.removeFromLeft (thirdWidth);
    modeSelector.setBounds (modeArea.withSizeKeepingCentre (juce::jmin (thirdWidth - 20, 160), 32));

    auto scArea = bottomArea;
    scIndicator.setBounds (scArea.withSizeKeepingCentre (70, 20));
}

void PlaceAudioProcessorEditor::timerCallback()
{
    bool scActive = processorRef.isSidechainActive();
    scIndicator.setText (scActive ? "SC ON" : "SC OFF", juce::dontSendNotification);

    float sizeVal = static_cast<float> (sizeMaximizerKnob.getSlider().getValue() / 100.0);
    stereoField.setSizeAmount (sizeVal);

    if (scActive)
        scPulsePhase += 0.05f;

    repaint();
}

void PlaceAudioProcessorEditor::generateNoiseTexture()
{
    noiseTexture = juce::Image (juce::Image::ARGB, 256, 256, false);
    juce::Graphics g (noiseTexture);

    for (int y = 0; y < 256; ++y)
    {
        for (int x = 0; x < 256; ++x)
        {
            float noise = static_cast<float> (juce::Random::getSystemRandom().nextInt (256)) / 255.0f;
            g.setColour (juce::Colours::white.withAlpha (noise * 0.015f));
            g.fillRect (x, y, 1, 1);
        }
    }
}

void PlaceAudioProcessorEditor::paintBackground (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient bgGrad (
        PlaceLookAndFeel::bgTop(), bounds.getCentreX(), bounds.getY(),
        PlaceLookAndFeel::bgBottom(), bounds.getCentreX(), bounds.getBottom(),
        false);
    bgGrad.addColour (0.5, PlaceLookAndFeel::bgMid());
    g.setGradientFill (bgGrad);
    g.fillAll();

    g.drawImage (noiseTexture, bounds, juce::RectanglePlacement::stretchToFit);
}

void PlaceAudioProcessorEditor::paintBloom (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centre = bounds.getCentre();

    for (int i = 5; i >= 0; --i)
    {
        float alpha = 0.012f * (1.0f - static_cast<float> (i) / 6.0f);
        float radius = 150.0f + static_cast<float> (i) * 20.0f;

        g.setColour (PlaceLookAndFeel::accent().withAlpha (alpha));
        g.fillEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);
    }
}

void PlaceAudioProcessorEditor::paintVignette (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    for (int i = 0; i < 12; ++i)
    {
        float alpha = 0.05f * (static_cast<float> (i) / 12.0f);
        float inset = static_cast<float> (i) * 12.0f;
        g.setColour (juce::Colours::black.withAlpha (alpha));
        g.drawRect (bounds.reduced (inset), 1.0f);
    }
}

void PlaceAudioProcessorEditor::paintGlassBorder (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    juce::Path borderPath;
    borderPath.addRoundedRectangle (bounds, 14.0f);

    g.setColour (PlaceLookAndFeel::glassBorder());
    g.strokePath (borderPath, juce::PathStrokeType (1.0f));
}

void PlaceAudioProcessorEditor::paintSCIndicator (juce::Graphics& g)
{
    bool scActive = processorRef.isSidechainActive();

    auto bounds = scIndicator.getBoundsInParent().toFloat();
    auto dotBounds = juce::Rectangle<float> (bounds.getX() - 12.0f, bounds.getCentreY() - 3.0f, 6.0f, 6.0f);

    if (scActive)
    {
        float pulse = (std::sin (scPulsePhase) + 1.0f) * 0.5f;
        float glowAlpha = 0.3f + pulse * 0.2f;

        for (int i = 3; i >= 0; --i)
        {
            float alpha = glowAlpha * (1.0f - static_cast<float> (i) / 4.0f);
            float expand = static_cast<float> (i) * 2.0f;
            g.setColour (PlaceLookAndFeel::cyanGlow().withAlpha (alpha));
            g.fillEllipse (dotBounds.expanded (expand));
        }

        g.setColour (PlaceLookAndFeel::cyanGlow());
    }
    else
    {
        g.setColour (PlaceLookAndFeel::textDim());
    }

    g.fillEllipse (dotBounds);
}
