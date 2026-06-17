#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

class ModeSelector : public juce::Component, private juce::Timer
{
public:
    ModeSelector();
    ~ModeSelector() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseEnter (const juce::MouseEvent& e) override;
    void mouseExit (const juce::MouseEvent& e) override;

    void setSelectedMode (int mode);
    int getSelectedMode() const { return selectedMode; }

    std::function<void(int)> onModeChanged;

private:
    int selectedMode = 0;
    float animationPos = 0.0f;
    float targetPos = 0.0f;
    bool hovered = false;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeSelector)
};
