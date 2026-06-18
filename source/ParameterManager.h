#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace ParamIDs
{
    static const juce::String SIZE_MAXIMIZER = "SIZE_MAXIMIZER";
    static const juce::String SIDE_BASS_REMOVER = "SIDE_BASS_REMOVER";
    static const juce::String MODE = "MODE";
    static const juce::String BYPASS = "BYPASS";
    static const juce::String SC_SOURCE = "SC_SOURCE"; // New Sidechain Parameter
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

        // NEW: Sidechain Source Dropdown
        auto scParamPtr = std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID { ParamIDs::SC_SOURCE, 1 },
            "Sidechain Source",
            juce::StringArray { "EXT SC", "INT SC" },
            1, // Defaults to index 1 ("INT SC")
            juce::AudioParameterChoiceAttributes());
        scSourceParam = scParamPtr.get();
        layout.add (std::move (scParamPtr));
    }

    float getSizeMaximizer() const noexcept { return sizeMaximizerParam->get(); }
    float getSideBassRemover() const noexcept { return sideBassRemoverParam->get(); }
    int getMode() const noexcept { return modeParam->getIndex(); }
    bool getBypass() const noexcept { return bypassParam->get(); }
    int getScSource() const noexcept { return scSourceParam->getIndex(); } // Getter for the dropdown

    float getSizeMaximizerNormalized() const noexcept { return sizeMaximizerParam->get() / 100.0f; }
    float getSideBassRemoverNormalized() const noexcept { return sideBassRemoverParam->get() / 100.0f; }

    float getHighPassFrequency() const noexcept
    {
        float normalized = getSideBassRemoverNormalized();
        return 20.0f + normalized * 70.0f;
    }

private:
    juce::AudioParameterFloat* sizeMaximizerParam = nullptr;
    juce::AudioParameterFloat* sideBassRemoverParam = nullptr;
    juce::AudioParameterChoice* modeParam = nullptr;
    juce::AudioParameterBool* bypassParam = nullptr;
    juce::AudioParameterChoice* scSourceParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterManager)
};