#pragma once

#include "PluginProcessor.h"
#include "ui/PlaceLookAndFeel.h"
#include "ui/RotaryKnob.h"
#include "ui/ModeSelector.h"

class PlaceAudioProcessorEditor : public juce::AudioProcessorEditor,
                                  private juce::Timer
{
public:
    explicit PlaceAudioProcessorEditor (PlaceAudioProcessor&);
    ~PlaceAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PlaceAudioProcessor& processorRef;
    PlaceLookAndFeel lookAndFeel;

    RotaryKnob sizeMaximizerKnob;
    RotaryKnob sideBassRemoverKnob;
    ModeSelector modeSelector;

    juce::Label scIndicator;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    void timerCallback() override;

    static constexpr int defaultWidth = 500;
    static constexpr int defaultHeight = 400;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};
