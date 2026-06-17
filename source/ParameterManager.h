#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace ParamIDs
{
    static const juce::String SIZE_MAXIMIZER = "SIZE_MAXIMIZER";
    static const juce::String SIDE_BASS_REMOVER = "SIDE_BASS_REMOVER";
    static const juce::String MODE = "MODE";
    static const juce::String BYPASS = "BYPASS";
}

namespace ModeValues
{
    static constexpr int STATIC_MODE = 0;
    static constexpr int VOCAL_FOLLOW_MODE = 1;
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

        auto modeParamPtr = std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID { ParamIDs::MODE, 1 },
            "Mode",
            juce::StringArray { "Static", "Vocal Follow" },
            0,
            juce::AudioParameterChoiceAttributes());
        modeParam = modeParamPtr.get();
        layout.add (std::move (modeParamPtr));

        auto bypassParamPtr = std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID { ParamIDs::BYPASS, 1 },
            "Bypass",
            false,
            juce::AudioParameterBoolAttributes());
        bypassParam = bypassParamPtr.get();
        layout.add (std::move (bypassParamPtr));
    }

    float getSizeMaximizer() const noexcept { return sizeMaximizerParam->get(); }
    float getSideBassRemover() const noexcept { return sideBassRemoverParam->get(); }
    int getMode() const noexcept { return modeParam->getIndex(); }
    bool getBypass() const noexcept { return bypassParam->get(); }

    float getSizeMaximizerNormalized() const noexcept { return sizeMaximizerParam->get() / 100.0f; }
    float getSideBassRemoverNormalized() const noexcept { return sideBassRemoverParam->get() / 100.0f; }

    float getHighPassFrequency() const noexcept
    {
        float normalized = getSideBassRemoverNormalized();
        return 20.0f + normalized * 160.0f;
    }

private:
    juce::AudioParameterFloat* sizeMaximizerParam = nullptr;
    juce::AudioParameterFloat* sideBassRemoverParam = nullptr;
    juce::AudioParameterChoice* modeParam = nullptr;
    juce::AudioParameterBool* bypassParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterManager)
};
