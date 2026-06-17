#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

class ModeSelector : public juce::Component
{
public:
    ModeSelector();
    ~ModeSelector() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;

    void setSelectedMode (int mode);
    int getSelectedMode() const { return selectedMode; }

    std::function<void(int)> onModeChanged;

private:
    int selectedMode = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeSelector)
};
