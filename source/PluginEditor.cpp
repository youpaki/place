#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
void MainPlaceKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto radius = (juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f) - 10.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();

    g.setColour(Theme::dimPurple.withAlpha(0.3f));
    g.drawEllipse(centreX - radius - 2, centreY - radius - 2, (radius + 2) * 2, (radius + 2) * 2, 2.0f);

    juce::DropShadow shadow(juce::Colours::black.withAlpha(0.8f), 8, juce::Point<int>(0, 5));
    juce::Path knobPath;
    knobPath.addEllipse(centreX - radius, centreY - radius, radius * 2, radius * 2);
    shadow.drawForPath(g, knobPath);

    juce::ColourGradient gradient(juce::Colour(0xff2d233d), centreX, centreY - radius,
                                  juce::Colour(0xff0d0812), centreX, centreY + radius, false);
    g.setGradientFill(gradient);
    g.fillPath(knobPath);

    g.setColour(juce::Colour(0xff3a2e4d));
    g.drawEllipse(centreX - radius, centreY - radius, radius * 2, radius * 2, 1.5f);

    auto startAngle = juce::MathConstants<float>::pi * 0.75f;
    auto endAngle   = juce::MathConstants<float>::pi * 2.25f;
    auto angle = juce::jmap((float)getValue(), (float)getMinimum(), (float)getMaximum(), startAngle, endAngle);
    
    auto dotRadius = 4.0f;
    auto dotDistance = radius * 0.7f;
    auto dotX = centreX + dotDistance * std::cos(angle);
    auto dotY = centreY + dotDistance * std::sin(angle);

    g.setColour(Theme::neonPurple.withAlpha(0.5f)); 
    g.fillEllipse(dotX - dotRadius * 2, dotY - dotRadius * 2, dotRadius * 4, dotRadius * 4);
    g.setColour(Theme::neonPurple); 
    g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2, dotRadius * 2);
}

void SideBassKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto radius = (juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f) - 6.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();

    auto mathStart = juce::MathConstants<float>::pi * 0.75f;
    auto mathEnd   = juce::MathConstants<float>::pi * 2.25f;
    auto arcStart  = mathStart + juce::MathConstants<float>::halfPi;
    auto arcEnd    = mathEnd + juce::MathConstants<float>::halfPi;
    auto currentMathAngle = juce::jmap((float)getValue(), (float)getMinimum(), (float)getMaximum(), mathStart, mathEnd);
    auto currentArcAngle  = currentMathAngle + juce::MathConstants<float>::halfPi;

    juce::Path bgArc;
    bgArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, arcStart, arcEnd, true);
    g.setColour(Theme::dimPurple.withAlpha(0.3f));
    g.strokePath(bgArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path activeArc;
    activeArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, arcStart, currentArcAngle, true);
    g.setColour(Theme::neonPurple);
    g.strokePath(activeArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto innerRadius = radius * 0.5f;
    g.setColour(Theme::knobDark);
    g.fillEllipse(centreX - innerRadius, centreY - innerRadius, innerRadius * 2, innerRadius * 2);
    
    auto dotX = centreX + innerRadius * 0.6f * std::cos(currentMathAngle);
    auto dotY = centreY + innerRadius * 0.6f * std::sin(currentMathAngle);
    g.setColour(Theme::neonPurple);
    g.fillEllipse(dotX - 1.5f, dotY - 1.5f, 3.0f, 3.0f);
}

void PillToggle::updateAnimation()
{
    float target = getToggleState() ? 1.0f : 0.0f;
    if (std::abs(target - currentPos) > 0.001f) {
        currentPos += (target - currentPos) * 0.15f; 
        repaint();
    }
}

void PillToggle::paintButton(juce::Graphics& g, bool, bool)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(Theme::bgMain);
    g.fillRoundedRectangle(bounds, bounds.getHeight() / 2.0f);
    g.setColour(Theme::panelOutline);
    g.drawRoundedRectangle(bounds, bounds.getHeight() / 2.0f, 1.5f);

    auto thumbRadius = bounds.getHeight() / 2.0f - 2.0f;
    auto minX = 2.0f;
    auto maxX = bounds.getWidth() - thumbRadius * 2.0f - 2.0f;
    auto thumbX = minX + currentPos * (maxX - minX);
    
    g.setColour(Theme::neonPurple.withAlpha(0.3f));
    g.fillEllipse(thumbX - 2.0f, 0.0f, thumbRadius * 2.0f + 4.0f, thumbRadius * 2.0f + 4.0f);
    g.setColour(Theme::neonPurple); 
    g.fillEllipse(thumbX, 2.0f, thumbRadius * 2.0f, thumbRadius * 2.0f);
}

void SegmentedMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(Theme::bgMain);
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(Theme::panelOutline);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

    int numSegments = 8;
    float padding = 4.0f;
    float segmentHeight = (bounds.getHeight() - (padding * (numSegments + 1))) / numSegments;
    float segmentWidth = bounds.getWidth() - (padding * 2);

    for (int i = 0; i < numSegments; ++i)
    {
        float y = bounds.getBottom() - padding - ((i + 1) * segmentHeight) - (i * padding);
        float threshold = (float)(i + 1) / numSegments;

        if (level >= threshold) {
            g.setColour(Theme::neonPurple);
            g.fillRoundedRectangle(bounds.getX() + padding, y, segmentWidth, segmentHeight, 2.0f);
            g.setColour(Theme::neonPurple.withAlpha(0.2f)); 
            g.fillRoundedRectangle(bounds.getX() + padding - 2, y - 2, segmentWidth + 4, segmentHeight + 4, 2.0f);
        } else {
            g.setColour(Theme::dimPurple.withAlpha(0.2f));
            g.fillRoundedRectangle(bounds.getX() + padding, y, segmentWidth, segmentHeight, 2.0f);
        }
    }
}

//==============================================================================
PlaceAudioProcessorEditor::PlaceAudioProcessorEditor (PlaceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(mainKnob);
    addAndMakeVisible(sideBassKnob);
    addAndMakeVisible(modeToggle);
    addAndMakeVisible(outMeter);
    addAndMakeVisible(scMeter);
    
    // Setup Sidechain Dropdown
    addAndMakeVisible(scSelector);
    scSelector.addItem("EXT SC", 1);
    scSelector.addItem("INT SC", 2);
    scSelector.setJustificationType(juce::Justification::centred);
    scSelector.setTextWhenNothingSelected("SOURCE");

    modeToggle.onClick = [this] { repaint(); };

    setSize (600, 400);
    startTimerHz(60);

    // ==============================================================================
    // Attachments (Remember to match exactly with your ParameterManager IDs!)
    // ==============================================================================
    mainKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "PLACE_AMOUNT", mainKnob);
    sideBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SIDE_BASS", sideBassKnob);
    modeToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "MODE", modeToggle);
    // scSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), "SC_SOURCE", scSelector);
}

PlaceAudioProcessorEditor::~PlaceAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

void PlaceAudioProcessorEditor::timerCallback()
{
    modeToggle.updateAnimation(); 
    
    outMeter.setLevel(audioProcessor.getCurrentLevel()); 
    
    // You will need to add a getter to your processor for this: `float getSidechainLevel() const noexcept`
    // scMeter.setLevel(audioProcessor.getSidechainLevel()); 
}

void PlaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (Theme::bgMain);
    auto panelBounds = getLocalBounds().toFloat().reduced(15.0f);
    g.setColour(Theme::bgPanel);
    g.fillRoundedRectangle(panelBounds, 8.0f);
    g.setColour(Theme::panelOutline);
    g.drawRoundedRectangle(panelBounds, 8.0f, 1.5f);

    // Header & Alignments (Perfectly aligned to the right edge X=580)
    g.setFont(juce::Font("Helvetica", 16.0f, juce::Font::bold).withExtraKerningFactor(0.2f));
    g.setColour(Theme::textLight);
    g.drawText("P L A C E", 0, 25, getWidth(), 30, juce::Justification::centred, true);
    
    g.setFont(juce::Font("Helvetica", 11.0f, juce::Font::plain).withExtraKerningFactor(0.1f));
    g.setColour(Theme::dimPurple);
    g.drawText("by leve", getWidth() - 100, 30, 80, 20, juce::Justification::right, true);
    g.drawText("v1.0.2",  getWidth() - 100, getHeight() - 40, 80, 20, juce::Justification::right, true);

    // Knob Labels
    g.drawText("P L A C E", mainKnob.getX(), mainKnob.getBottom() + 15, mainKnob.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("SIDE BASS\nREMOVER", sideBassKnob.getX() - 20, sideBassKnob.getBottom() + 10, sideBassKnob.getWidth() + 40, 30, juce::Justification::centred, true);

    // Toggle Labels
    if (!modeToggle.getToggleState()) g.setColour(Theme::neonPurple); else g.setColour(Theme::dimPurple);
    g.drawText("STATIC", modeToggle.getX() - 60, modeToggle.getY(), 50, modeToggle.getHeight(), juce::Justification::centredRight, true);
    if (modeToggle.getToggleState()) g.setColour(Theme::neonPurple); else g.setColour(Theme::dimPurple);
    g.drawText("VOCAL FOLLOW", modeToggle.getRight() + 10, modeToggle.getY(), 100, modeToggle.getHeight(), juce::Justification::centredLeft, true);

    // Meter Column Labels
    g.setColour(Theme::dimPurple);
    g.setFont(juce::Font("Helvetica", 10.0f, juce::Font::bold).withExtraKerningFactor(0.1f));
    g.drawText("SC", scMeter.getX(), scMeter.getY() - 22, scMeter.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("OUT", outMeter.getX(), outMeter.getY() - 22, outMeter.getWidth(), 20, juce::Justification::centred, true);
}

void PlaceAudioProcessorEditor::resized()
{
// ==============================================================================
    // Attachments linked directly to ParamIDs
    // ==============================================================================
    mainKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::SIZE_MAXIMIZER, mainKnob);
        
    sideBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::SIDE_BASS_REMOVER, sideBassKnob);
        
    modeToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::MODE, modeToggle);
        
    scSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::SC_SOURCE, scSelector);
}