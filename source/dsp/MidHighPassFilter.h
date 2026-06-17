#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <cmath>

class MidHighPassFilter
{
public:
    MidHighPassFilter() = default;

    void prepare (double sampleRate, int /*samplesPerBlock*/)
    {
        currentSampleRate = sampleRate;
        reset();
        updateCoefficients (currentFrequency);
    }

    void process (float* channel, int numSamples, float frequency) noexcept
    {
        if (std::abs (frequency - currentFrequency) > 0.1f)
        {
            currentFrequency = frequency;
            updateCoefficients (currentSampleRate);
        }

        for (int i = 0; i < numSamples; ++i)
        {
            float sample = channel[i];
            sample = processBiquad (sample, coeffs1, state1);
            sample = processBiquad (sample, coeffs2, state2);
            channel[i] = sample;
        }
    }

    void reset() noexcept
    {
        state1 = { 0.0f, 0.0f, 0.0f, 0.0f };
        state2 = { 0.0f, 0.0f, 0.0f, 0.0f };
    }

private:
    struct BiquadCoeffs
    {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    };

    struct BiquadState
    {
        float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
    };

    BiquadCoeffs coeffs1, coeffs2;
    BiquadState state1, state2;
    double currentSampleRate = 44100.0;
    float currentFrequency = 20.0f;

    static float processBiquad (float x, const BiquadCoeffs& c, BiquadState& s) noexcept
    {
        float y = c.b0 * x + c.b1 * s.x1 + c.b2 * s.x2 - c.a1 * s.y1 - c.a2 * s.y2;
        s.x2 = s.x1;
        s.x1 = x;
        s.y2 = s.y1;
        s.y1 = y;
        return y;
    }

    void updateCoefficients (double sampleRate, float freq = -1.0f)
    {
        if (freq > 0.0f)
            currentFrequency = freq;

        auto butterworth = calcButterworthHP (static_cast<float> (sampleRate), currentFrequency);
        coeffs1 = butterworth;
        coeffs2 = butterworth;
    }

    static BiquadCoeffs calcButterworthHP (float fs, float freq)
    {
        BiquadCoeffs c;

        if (freq < 10.0f)
        {
            c = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            return c;
        }

        float w0 = 2.0f * juce::MathConstants<float>::pi * freq / fs;
        float sinW0 = std::sin (w0);
        float cosW0 = std::cos (w0);
        float alpha = sinW0 / (2.0f * std::sqrt (2.0f));

        float a0 = 1.0f + alpha;
        float invA0 = 1.0f / a0;

        float halfCos = (1.0f + cosW0) * 0.5f;

        c.b0 = halfCos * invA0;
        c.b1 = -(1.0f + cosW0) * invA0;
        c.b2 = halfCos * invA0;
        c.a1 = (-2.0f * cosW0) * invA0;
        c.a2 = (1.0f - alpha) * invA0;

        return c;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidHighPassFilter)
};
