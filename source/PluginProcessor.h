#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ParameterManager.h"
#include "dsp/MidSideProcessor.h"
#include "dsp/StaticVoiceSpaceEQ.h"
#include "dsp/DynamicVoiceSpaceEQ.h"
#include "dsp/SideCompensationEQ.h"
#include "dsp/MidHighPassFilter.h"
#include "dsp/SidechainDetector.h"

class PlaceAudioProcessor : public juce::AudioProcessor
{
public:
    PlaceAudioProcessor();
    ~PlaceAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "PLACE"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    ParameterManager& getParameterManager() { return *paramManager; }

    bool isSidechainActive() const noexcept { return sidechainConnected; }

private:
    std::unique_ptr<ParameterManager> paramManager;
    juce::AudioProcessorValueTreeState apvts;

    MidSideProcessor midSideProcessor;
    StaticVoiceSpaceEQ staticEQ;
    DynamicVoiceSpaceEQ dynamicEQ;
    SideCompensationEQ sideCompensation;
    MidHighPassFilter midHighPass;
    SidechainDetector sidechainDetector;

    float smoothedSize = 0.0f;
    float smoothedBass = 0.0f;
    float smoothCoeff = 0.999f;

    bool sidechainConnected = false;

    void updateSmoothing (float targetSize, float targetBass, int numSamples) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessor)
};
