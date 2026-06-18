#include "PluginEditor.h"

PlaceAudioProcessorEditor::PlaceAudioProcessorEditor (PlaceAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    setLookAndFeel (&lookAndFeel);
    setResizable (true, true);
    setResizeLimits (480, 270, 1280, 720);

    addAndMakeVisible (stereoField);
    addAndMakeVisible (mainKnob);
    addAndMakeVisible (sideBassKnob);
    addAndMakeVisible (modeToggle);
    addAndMakeVisible (meter);

    scIndicator.setText ("SC OFF", juce::dontSendNotification);
    scIndicator.setJustificationType (juce::Justification::centred);
    scIndicator.setFont (juce::FontOptions (10.0f));
    scIndicator.setColour (juce::Label::textColourId, PlaceLookAndFeel::textDim());
    addAndMakeVisible (scIndicator);

    mainKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIZE_MAXIMIZER, mainKnob.getSlider());

    sideBassKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIDE_BASS_REMOVER, sideBassKnob.getSlider());

    modeToggle.onToggle = [this](bool isStatic)
    {
        if (auto* param = processorRef.getAPVTS().getParameter (ParamIDs::MODE))
            param->setValueNotifyingHost (isStatic ? 0.0f : 1.0f);
    };

    if (auto* modeParam = processorRef.getAPVTS().getParameter (ParamIDs::MODE))
    {
        bool isStatic = (modeParam->getValue() < 0.5f);
        modeToggle.isStatic = isStatic;
        modeToggle.repaint();
    }

    setSize (defaultWidth, defaultHeight);
    startTimerHz (30);
}

PlaceAudioProcessorEditor::~PlaceAudioProcessorEditor()
{
    mainKnobAttachment.reset();
    sideBassKnobAttachment.reset();
    setLookAndFeel (nullptr);
}

void PlaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient bgGrad (
        PlaceLookAndFeel::bgTop(), bounds.getCentreX(), bounds.getY(),
        PlaceLookAndFeel::bgBottom(), bounds.getCentreX(), bounds.getBottom(),
        false);
    g.setGradientFill (bgGrad);
    g.fillAll();

    g.setColour (PlaceLookAndFeel::textPrimary());
    g.setFont (juce::FontOptions (22.0f));
    g.drawFittedText ("P L A C E", getLocalBounds().removeFromTop (42), juce::Justification::centred, 1);

    g.setColour (PlaceLookAndFeel::textSecondary());
    g.setFont (juce::FontOptions (11.0f));
    g.drawFittedText ("by leve", getLocalBounds().removeFromTop (22).removeFromTop (12),
                      juce::Justification::centred, 1);

    bool scActive = processorRef.isSidechainActive();
    auto scBounds = scIndicator.getBoundsInParent().toFloat();
    auto dotBounds = juce::Rectangle<float> (scBounds.getX() - 10.0f, scBounds.getCentreY() - 2.5f, 5.0f, 5.0f);

    g.setColour (scActive ? PlaceLookAndFeel::cyan() : PlaceLookAndFeel::textDim());
    g.fillEllipse (dotBounds);

    g.setColour (PlaceLookAndFeel::textDim());
    g.setFont (juce::FontOptions (9.0f));
    g.drawFittedText ("v1.0", getLocalBounds().removeFromBottom (16).removeFromRight (45),
                      juce::Justification::centredRight, 1);
}

void PlaceAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (55);

    int knobSize = juce::jmin (area.getWidth() / 2, area.getHeight() / 2);
    knobSize = juce::jmax (knobSize, 100);

    auto centerArea = area.withSizeKeepingCentre (knobSize, knobSize + 35);
    stereoField.setBounds (centerArea.expanded (30));
    mainKnob.setBounds (centerArea);

    area.removeFromTop (centerArea.getHeight());

    int smallKnobSize = juce::jmax (knobSize / 3, 55);
    int bottomHeight = juce::jmax (smallKnobSize + 25, 70);
    auto bottomArea = area.removeFromBottom (bottomHeight);

    int thirdWidth = bottomArea.getWidth() / 3;

    auto bassArea = bottomArea.removeFromLeft (thirdWidth);
    sideBassKnob.setBounds (bassArea.withSizeKeepingCentre (smallKnobSize, smallKnobSize + 20));

    auto modeArea = bottomArea.removeFromLeft (thirdWidth);
    modeToggle.setBounds (modeArea.withSizeKeepingCentre (juce::jmin (thirdWidth - 20, 150), 28));

    auto meterArea = bottomArea;
    meter.setBounds (meterArea.withSizeKeepingCentre (juce::jmin (thirdWidth - 20, 120), 12));

    auto scArea = meterArea.removeFromBottom (20);
    scIndicator.setBounds (scArea.withSizeKeepingCentre (60, 18));
}

void PlaceAudioProcessorEditor::timerCallback()
{
    bool scActive = processorRef.isSidechainActive();
    scIndicator.setText (scActive ? "SC ON" : "SC OFF", juce::dontSendNotification);

    float sizeVal = static_cast<float> (mainKnob.getSlider().getValue() / 100.0);
    stereoField.setSizeAmount (sizeVal);

    float level = processorRef.getCurrentLevel();
    meter.setLevel (level);

    repaint();
}
