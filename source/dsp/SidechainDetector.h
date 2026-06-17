#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <cmath>

class SidechainDetector
{
public:
    SidechainDetector() = default;

    void prepare (double sampleRate, int samplesPerBlock)
    {
        currentSampleRate = sampleRate;
        blockSize = samplesPerBlock;

        for (size_t i = 0; i < numBands; ++i)
        {
            bpStates[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
            envelopes[i] = 0.0f;
        }

        updateBandpassCoeffs();

        float attackSec = 0.005f;
        float releaseSec = 0.080f;
        attackCoeff = std::exp (-1.0f / (attackSec * static_cast<float> (sampleRate)));
        releaseCoeff = std::exp (-1.0f / (releaseSec * static_cast<float> (sampleRate)));
    }

    void analyze (const float* leftChannel, const float* rightChannel, int numSamples) noexcept
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float mono = (leftChannel[i] + rightChannel[i]) * 0.5f;

            for (size_t b = 0; b < numBands; ++b)
            {
                float filtered = processBiquad (mono, bpCoeffs[b], bpStates[b]);
                float squared = filtered * filtered;

                float env = envelopes[b];
                if (squared > env)
                    env = attackCoeff * env + (1.0f - attackCoeff) * squared;
                else
                    env = releaseCoeff * env + (1.0f - releaseCoeff) * squared;

                envelopes[b] = env;
            }
        }
    }

    float getEnvelope (size_t band) const noexcept
    {
        return band < numBands ? std::sqrt (envelopes[band]) : 0.0f;
    }

    void reset() noexcept
    {
        for (size_t i = 0; i < numBands; ++i)
        {
            bpStates[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
            envelopes[i] = 0.0f;
        }
    }

    static constexpr size_t numBands = 6;
    static constexpr std::array<float, numBands> bandFreqs = { 250.0f, 500.0f, 1000.0f, 2500.0f, 5000.0f, 8000.0f };

private:
    struct BiquadCoeffs
    {
        float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    };

    struct BiquadState
    {
        float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
    };

    static constexpr std::array<float, numBands> qValues = { 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f };

    std::array<BiquadCoeffs, numBands> bpCoeffs;
    std::array<BiquadState, numBands> bpStates;
    std::array<float, numBands> envelopes;

    double currentSampleRate = 44100.0;
    int blockSize = 512;
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;

    static float processBiquad (float x, const BiquadCoeffs& c, BiquadState& s) noexcept
    {
        float y = c.b0 * x + c.b1 * s.x1 + c.b2 * s.x2 - c.a1 * s.y1 - c.a2 * s.y2;
        s.x2 = s.x1;
        s.x1 = x;
        s.y2 = s.y1;
        s.y1 = y;
        return y;
    }

    void updateBandpassCoeffs()
    {
        for (size_t i = 0; i < numBands; ++i)
            bpCoeffs[i] = calcBandpass (static_cast<float> (currentSampleRate), bandFreqs[i], qValues[i]);
    }

    static BiquadCoeffs calcBandpass (float fs, float freq, float Q)
    {
        BiquadCoeffs c;
        float w0 = 2.0f * juce::MathConstants<float>::pi * freq / fs;
        float sinW0 = std::sin (w0);
        float cosW0 = std::cos (w0);
        float alpha = sinW0 / (2.0f * Q);

        float a0 = 1.0f + alpha;
        float invA0 = 1.0f / a0;

        c.b0 = alpha * invA0;
        c.b1 = 0.0f;
        c.b2 = -alpha * invA0;
        c.a1 = (-2.0f * cosW0) * invA0;
        c.a2 = (1.0f - alpha) * invA0;

        return c;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidechainDetector)
};
