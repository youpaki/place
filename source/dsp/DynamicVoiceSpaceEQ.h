#pragma once

#include "SidechainDetector.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <cmath>

class DynamicVoiceSpaceEQ
{
public:
    DynamicVoiceSpaceEQ() = default;

    void prepare (double sampleRate, int /*samplesPerBlock*/)
    {
        currentSampleRate = sampleRate;

        for (auto& state : filterStates)
            state = { 0.0f, 0.0f, 0.0f, 0.0f };

        for (auto& g : smoothedGains)
            g = 1.0f;

        float smoothTimeSec = 0.002f;
        smoothCoeff = std::exp (-1.0f / (smoothTimeSec * static_cast<float> (sampleRate)));

        updateBaseCoeffs();
    }

    void process (float* midChannel, int numSamples, float sizeAmount, const SidechainDetector& detector) noexcept
    {
        for (size_t b = 0; b < numBands; ++b)
        {
            float envLevel = detector.getEnvelope (b);
            float sensitivity = 5.0f;
            float scaledLevel = std::min (envLevel * sensitivity, 1.0f);

            float maxAttenDB = maxAttenDBValues[b] * sizeAmount;
            float targetGainDB = maxAttenDB * scaledLevel;

            float targetA = (std::abs (targetGainDB) < 0.01f) ? 1.0f : std::pow (10.0f, targetGainDB / 40.0f);
            smoothedGains[b] = smoothCoeff * smoothedGains[b] + (1.0f - smoothCoeff) * targetA;

            float A = smoothedGains[b];
            float w0 = 2.0f * juce::MathConstants<float>::pi * bandFreqs[b] / static_cast<float> (currentSampleRate);
            float cosW0 = std::cos (w0);
            float sinW0 = std::sin (w0);
            float alpha = sinW0 / (2.0f * qValues[b]);

            float a0 = 1.0f + alpha / A;
            float invA0 = 1.0f / a0;

            BiquadCoeffs dynamicCoeffs;
            dynamicCoeffs.b0 = (1.0f + alpha * A) * invA0;
            dynamicCoeffs.b1 = (-2.0f * cosW0) * invA0;
            dynamicCoeffs.b2 = (1.0f - alpha * A) * invA0;
            dynamicCoeffs.a1 = (-2.0f * cosW0) * invA0;
            dynamicCoeffs.a2 = (1.0f - alpha / A) * invA0;

            for (int i = 0; i < numSamples; ++i)
                midChannel[i] = processBiquad (midChannel[i], dynamicCoeffs, filterStates[b]);
        }
    }

    void reset() noexcept
    {
        for (auto& state : filterStates)
            state = { 0.0f, 0.0f, 0.0f, 0.0f };
        for (auto& g : smoothedGains)
            g = 1.0f;
    }

private:
    static constexpr size_t numBands = 6;

    struct BiquadCoeffs
    {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    };

    struct BiquadState
    {
        float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
    };

    static constexpr std::array<float, numBands> bandFreqs = { 250.0f, 500.0f, 1000.0f, 2500.0f, 5000.0f, 8000.0f };
    static constexpr std::array<float, numBands> maxAttenDBValues = { -3.0f, -6.0f, -8.0f, -12.0f, -10.0f, -8.0f };
    static constexpr std::array<float, numBands> qValues = { 1.0f, 1.2f, 1.4f, 1.5f, 1.2f, 1.0f };

    std::array<BiquadCoeffs, numBands> baseCoeffs;
    std::array<BiquadState, numBands> filterStates;
    std::array<float, numBands> smoothedGains;
    double currentSampleRate = 44100.0;
    float smoothCoeff = 0.99f;

    static float processBiquad (float x, const BiquadCoeffs& c, BiquadState& s) noexcept
    {
        float y = c.b0 * x + c.b1 * s.x1 + c.b2 * s.x2 - c.a1 * s.y1 - c.a2 * s.y2;
        s.x2 = s.x1;
        s.x1 = x;
        s.y2 = s.y1;
        s.y1 = y;
        return y;
    }

    void updateBaseCoeffs()
    {
        for (size_t i = 0; i < numBands; ++i)
        {
            float w0 = 2.0f * juce::MathConstants<float>::pi * bandFreqs[i] / static_cast<float> (currentSampleRate);
            float cosW0 = std::cos (w0);
            float sinW0 = std::sin (w0);
            float alpha = sinW0 / (2.0f * qValues[i]);

            float a0 = 1.0f + alpha;
            float invA0 = 1.0f / a0;

            baseCoeffs[i].b0 = 1.0f * invA0;
            baseCoeffs[i].b1 = (-2.0f * cosW0) * invA0;
            baseCoeffs[i].b2 = 1.0f * invA0;
            baseCoeffs[i].a1 = (-2.0f * cosW0) * invA0;
            baseCoeffs[i].a2 = (1.0f - alpha) * invA0;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicVoiceSpaceEQ)
};
