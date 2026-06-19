#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <cmath>

class StaticVoiceSpaceEQ
{
public:
    StaticVoiceSpaceEQ() = default;

    void prepare (double sampleRate, int /*samplesPerBlock*/)
    {
        currentSampleRate = sampleRate;

        for (auto& state : filterStates)
            state = { 0.0f, 0.0f };

        updateCoefficients (currentAmount);
    }

    void process (float* midChannel, int numSamples, float amount) noexcept
    {
        if (std::abs (amount - currentAmount) > 1e-6f)
        {
            currentAmount = amount;
            updateCoefficients (currentSampleRate);
        }

        for (int i = 0; i < numSamples; ++i)
        {
            float sample = midChannel[i];
            for (size_t b = 0; b < numBands; ++b)
                sample = processBiquad (sample, coeffs[b], filterStates[b]);
            midChannel[i] = sample;
        }
    }

    void reset() noexcept
    {
        for (auto& state : filterStates)
            state = { 0.0f, 0.0f };
    }

private:
    static constexpr size_t numBands = 7;

    struct BiquadCoeffs
    {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    };

    struct BiquadState
    {
        float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
    };

    static constexpr std::array<float, numBands> bandFreqs = { 250.0f, 500.0f, 1000.0f, 2500.0f, 5000.0f, 8000.0f, 12000.0f };
    static constexpr std::array<float, numBands> maxAttenDB = { -3.0f, -6.0f, -8.0f, -12.0f, -10.0f, -8.0f, -4.0f };
    static constexpr std::array<float, numBands> qValues = { 1.0f, 1.2f, 1.4f, 1.5f, 1.2f, 1.0f, 0.7f };

    std::array<BiquadCoeffs, numBands> coeffs;
    std::array<BiquadState, numBands> filterStates;
    double currentSampleRate = 44100.0;
    float currentAmount = 0.0f;

    static float processBiquad (float x, const BiquadCoeffs& c, BiquadState& s) noexcept
    {
        float y = c.b0 * x + c.b1 * s.x1 + c.b2 * s.x2 - c.a1 * s.y1 - c.a2 * s.y2;
        s.x2 = s.x1;
        s.x1 = x;
        s.y2 = s.y1;
        s.y1 = y;
        return y;
    }

    void updateCoefficients (float amount)
    {
        updateCoefficients (currentSampleRate, amount);
    }

    void updateCoefficients (double sampleRate, float amount = -1.0f)
    {
        if (amount >= 0.0f)
            currentAmount = amount;

        for (size_t i = 0; i < numBands - 1; ++i)
        {
            float gainDB = maxAttenDB[i] * currentAmount;
            coeffs[i] = calcPeakingEQ (static_cast<float> (sampleRate), bandFreqs[i], qValues[i], gainDB);
        }

        float shelfGainDB = maxAttenDB[numBands - 1] * currentAmount;
        coeffs[numBands - 1] = calcHighShelf (static_cast<float> (sampleRate), bandFreqs[numBands - 1], shelfGainDB);
    }

    static BiquadCoeffs calcPeakingEQ (float fs, float freq, float Q, float gainDB)
    {
        BiquadCoeffs c;

        if (std::abs (gainDB) < 0.001f)
        {
            c = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            return c;
        }

        float A = std::pow (10.0f, gainDB / 40.0f);
        float w0 = 2.0f * juce::MathConstants<float>::pi * freq / fs;
        float sinW0 = std::sin (w0);
        float cosW0 = std::cos (w0);
        float alpha = sinW0 / (2.0f * Q);

        float a0 = 1.0f + alpha / A;
        float invA0 = 1.0f / a0;

        c.b0 = (1.0f + alpha * A) * invA0;
        c.b1 = (-2.0f * cosW0) * invA0;
        c.b2 = (1.0f - alpha * A) * invA0;
        c.a1 = (-2.0f * cosW0) * invA0;
        c.a2 = (1.0f - alpha / A) * invA0;

        return c;
    }

    static BiquadCoeffs calcHighShelf (float fs, float freq, float gainDB)
    {
        BiquadCoeffs c;

        if (std::abs (gainDB) < 0.001f)
        {
            c = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            return c;
        }

        float A = std::pow (10.0f, gainDB / 40.0f);
        float w0 = 2.0f * juce::MathConstants<float>::pi * freq / fs;
        float sinW0 = std::sin (w0);
        float cosW0 = std::cos (w0);
        float alpha = sinW0 / 2.0f * std::sqrt ((A + 1.0f / A) * 2.0f);

        float a0 = (A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * std::sqrt (A) * alpha;
        float invA0 = 1.0f / a0;

        c.b0 = A * ((A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * std::sqrt (A) * alpha) * invA0;
        c.b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosW0) * invA0;
        c.b2 = A * ((A + 1.0f) + (A - 1.0f) * cosW0 - 2.0f * std::sqrt (A) * alpha) * invA0;
        c.a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cosW0) * invA0;
        c.a2 = ((A + 1.0f) - (A - 1.0f) * cosW0 - 2.0f * std::sqrt (A) * alpha) * invA0;

        return c;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StaticVoiceSpaceEQ)
};
