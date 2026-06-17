#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MidSideProcessor
{
public:
    MidSideProcessor() = default;

    static void encode (juce::AudioBuffer<float>& buffer, int numSamples) noexcept
    {
        auto* left = buffer.getWritePointer (0);
        auto* right = buffer.getWritePointer (1);

        for (int i = 0; i < numSamples; ++i)
        {
            float mid = (left[i] + right[i]) * 0.5f;
            float side = (left[i] - right[i]) * 0.5f;
            left[i] = mid;
            right[i] = side;
        }
    }

    static void decode (juce::AudioBuffer<float>& buffer, int numSamples) noexcept
    {
        auto* mid = buffer.getWritePointer (0);
        auto* side = buffer.getWritePointer (1);

        for (int i = 0; i < numSamples; ++i)
        {
            float l = mid[i] + side[i];
            float r = mid[i] - side[i];
            mid[i] = l;
            side[i] = r;
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidSideProcessor)
};
