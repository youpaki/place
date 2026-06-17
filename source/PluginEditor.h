#pragma once

#include "PluginProcessor.h"
#include "ui/PlaceLookAndFeel.h"
#include "ui/RotaryKnob.h"
#include "ui/ModeSelector.h"
#include "ui/StereoFieldVisualizer.h"

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

    PremiumKnob sizeMaximizerKnob;
    PremiumKnob sideBassRemoverKnob;
    ModeSelector modeSelector;
    StereoFieldVisualizer stereoField;

    juce::Label scIndicator;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;

    juce::Image noiseTexture;

    void timerCallback() override;
    void generateNoiseTexture();
    void paintBackground (juce::Graphics& g);
    void paintBloom (juce::Graphics& g);
    void paintVignette (juce::Graphics& g);
    void paintGlassBorder (juce::Graphics& g);
    void paintSCIndicator (juce::Graphics& g);

    float scPulsePhase = 0.0f;

    static constexpr int defaultWidth = 640;
    static constexpr int defaultHeight = 360;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};
