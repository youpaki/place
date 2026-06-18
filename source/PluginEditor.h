#pragma once

#include "PluginProcessor.h"
#include "ui/PlaceLookAndFeel.h"
#include "ui/RotaryKnob.h"
#include "ui/ModeSelector.h"
#include "ui/StereoFieldVisualizer.h"

class MainPlaceKnob : public PremiumKnob
{
public:
    MainPlaceKnob() : PremiumKnob ("SIZE MAXIMIZER", true) {}
};

class SideBassKnob : public PremiumKnob
{
public:
    SideBassKnob() : PremiumKnob ("SIDE BASS REMOVER", false) {}
};

class ModeToggle : public juce::Component
{
public:
    ModeToggle()
    {
        setRepaintsOnMouseActivity (true);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (0.5f);

        juce::Path pillPath;
        pillPath.addRoundedRectangle (bounds, bounds.getHeight() * 0.5f);

        g.setColour (juce::Colour (0xff3d2d5a));
        g.fillPath (pillPath);

        g.setColour (juce::Colour (0x20ffffff));
        g.strokePath (pillPath, juce::PathStrokeType (1.0f));

        float halfWidth = bounds.getWidth() * 0.5f;
        auto activeBounds = isStatic
            ? juce::Rectangle<float> (bounds.getX(), bounds.getY(), halfWidth, bounds.getHeight())
            : juce::Rectangle<float> (bounds.getX() + halfWidth, bounds.getY(), halfWidth, bounds.getHeight());

        juce::Path activePath;
        activePath.addRoundedRectangle (activeBounds.reduced (2.0f), activeBounds.getHeight() * 0.45f);

        g.setColour (juce::Colour (0xff8b5cf6).withAlpha (0.25f));
        g.fillPath (activePath);

        g.setFont (juce::FontOptions (10.0f));

        auto leftTextBounds = juce::Rectangle<float> (bounds.getX(), bounds.getY(), halfWidth, bounds.getHeight());
        auto rightTextBounds = juce::Rectangle<float> (bounds.getX() + halfWidth, bounds.getY(), halfWidth, bounds.getHeight());

        g.setColour (isStatic ? juce::Colour (0xffe8e0f0) : juce::Colour (0x55e8e0f0));
        g.drawFittedText ("STATIC", leftTextBounds.toNearestInt(), juce::Justification::centred, 1);

        g.setColour (!isStatic ? juce::Colour (0xffe8e0f0) : juce::Colour (0x55e8e0f0));
        g.drawFittedText ("VOCAL FOLLOW", rightTextBounds.toNearestInt(), juce::Justification::centred, 1);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        auto bounds = getLocalBounds().toFloat();
        float halfWidth = bounds.getWidth() * 0.5f;

        bool newIsStatic = (e.x < halfWidth);

        if (newIsStatic != isStatic)
        {
            isStatic = newIsStatic;
            repaint();

            if (onToggle)
                onToggle (isStatic);
        }
    }

    bool isStatic = true;
    std::function<void(bool)> onToggle;
};

class SegmentMeter : public juce::Component
{
public:
    void setLevel (float newLevel)
    {
        if (std::abs (newLevel - level) > 0.01f)
        {
            level = newLevel;
            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        g.setColour (juce::Colour (0xff3d2d5a));
        g.fillRoundedRectangle (bounds, 2.0f);

        float meterWidth = bounds.getWidth() * level;
        if (meterWidth > 0.0f)
        {
            g.setColour (juce::Colour (0xff8b5cf6));
            g.fillRoundedRectangle (bounds.getX(), bounds.getY(), meterWidth, bounds.getHeight(), 2.0f);
        }
    }

private:
    float level = 0.0f;
};

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

    MainPlaceKnob mainKnob;
    SideBassKnob sideBassKnob;
    ModeToggle modeToggle;
    SegmentMeter meter;
    StereoFieldVisualizer stereoField;

    juce::Label scIndicator;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mainKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideBassKnobAttachment;

    void timerCallback() override;

    static constexpr int defaultWidth = 640;
    static constexpr int defaultHeight = 360;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceAudioProcessorEditor)
};
