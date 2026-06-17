#include "PluginEditor.h"

PlaceAudioProcessorEditor::PlaceAudioProcessorEditor (PlaceAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p),
      sizeMaximizerKnob ("SIZE MAXIMIZER"),
      sideBassRemoverKnob ("SIDE BASS REMOVER")
{
    setLookAndFeel (&lookAndFeel);

    setResizable (true, true);
    setResizeLimits (350, 280, 1000, 800);

    addAndMakeVisible (sizeMaximizerKnob);
    addAndMakeVisible (sideBassRemoverKnob);
    addAndMakeVisible (modeSelector);

    scIndicator.setText ("SC OFF", juce::dontSendNotification);
    scIndicator.setJustificationType (juce::Justification::centred);
    scIndicator.setFont (juce::FontOptions (12.0f));
    addAndMakeVisible (scIndicator);

    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIZE_MAXIMIZER, sizeMaximizerKnob.getSlider());

    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getAPVTS(), ParamIDs::SIDE_BASS_REMOVER, sideBassRemoverKnob.getSlider());

    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.getAPVTS(), ParamIDs::MODE, modeSelector.getComboBox());

    setSize (defaultWidth, defaultHeight);
    startTimerHz (30);
}

PlaceAudioProcessorEditor::~PlaceAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void PlaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bg = juce::Colour (0xff1a0a2e);
    g.fillAll (bg);

    auto area = getLocalBounds();

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (28.0f, juce::Font::bold));
    g.drawFittedText ("PLACE", area.removeFromTop (50), juce::Justification::centred, 1);

    g.setFont (juce::FontOptions (13.0f));
    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.drawFittedText ("by leve", area.removeFromTop (25), juce::Justification::centred, 1);

    g.setFont (juce::FontOptions (11.0f));
    g.setColour (juce::Colours::white.withAlpha (0.3f));
    g.drawFittedText ("v1.0", getLocalBounds().removeFromBottom (20).removeFromRight (60),
                      juce::Justification::centredRight, 1);
}

void PlaceAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (75);

    int knobSize = juce::jmin (area.getWidth() / 2, area.getHeight() / 2);
    knobSize = juce::jmax (knobSize, 80);

    auto centerArea = area.withSizeKeepingCentre (knobSize, knobSize + 30);
    sizeMaximizerKnob.setBounds (centerArea);

    area.removeFromTop (centerArea.getHeight());

    int smallKnobSize = juce::jmax (knobSize / 2, 60);
    int bottomHeight = juce::jmax (smallKnobSize + 30, 80);
    auto bottomArea = area.removeFromBottom (bottomHeight);

    int thirdWidth = bottomArea.getWidth() / 3;

    auto bassArea = bottomArea.removeFromLeft (thirdWidth);
    sideBassRemoverKnob.setBounds (bassArea.withSizeKeepingCentre (smallKnobSize, smallKnobSize + 25));

    auto modeArea = bottomArea.removeFromLeft (thirdWidth);
    modeSelector.setBounds (modeArea.withSizeKeepingCentre (juce::jmin (thirdWidth - 10, 140), 30));

    auto scArea = bottomArea;
    scIndicator.setBounds (scArea.withSizeKeepingCentre (60, 20));
}

void PlaceAudioProcessorEditor::timerCallback()
{
    bool scActive = processorRef.isSidechainActive();
    scIndicator.setText (scActive ? "SC ON" : "SC OFF", juce::dontSendNotification);
    scIndicator.setColour (juce::Label::textColourId,
                           scActive ? juce::Colour (0xff9b59b6) : juce::Colours::white.withAlpha (0.3f));
}
