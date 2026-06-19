#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace ParamIDs
{
    static const juce::String SIZE_MAXIMIZER = "SIZE_MAXIMIZER";
    static const juce::String SIDE_BASS_REMOVER = "SIDE_BASS_REMOVER";
}

class ParameterManager
{
public:
    explicit ParameterManager (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    {
        auto sizeParam = std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { ParamIDs::SIZE_MAXIMIZER, 1 },
            "Size Maximizer",
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("%"));
        sizeMaximizerParam = sizeParam.get();
        layout.add (std::move (sizeParam));

        auto bassParam = std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { ParamIDs::SIDE_BASS_REMOVER, 1 },
            "Side Bass Remover",
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("%"));
        sideBassRemoverParam = bassParam.get();
        layout.add (std::move (bassParam));
    }

    float getSizeMaximizer() const noexcept { return sizeMaximizerParam->get(); }
    float getSideBassRemover() const noexcept { return sideBassRemoverParam->get(); }

    float getSizeMaximizerNormalized() const noexcept { return sizeMaximizerParam->get() / 100.0f; }
    float getSideBassRemoverNormalized() const noexcept { return sideBassRemoverParam->get() / 100.0f; }

private:
    juce::AudioParameterFloat* sizeMaximizerParam = nullptr;
    juce::AudioParameterFloat* sideBassRemoverParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterManager)
};
