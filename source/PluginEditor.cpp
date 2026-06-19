#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterManager.h"

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

PlaceAudioProcessorEditor::PlaceAudioProcessorEditor (PlaceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(mainKnob);
    addAndMakeVisible(sideBassKnob);

    setSize (400, 350);

    mainKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::SIZE_MAXIMIZER, mainKnob);

    sideBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), ParamIDs::SIDE_BASS_REMOVER, sideBassKnob);
}

PlaceAudioProcessorEditor::~PlaceAudioProcessorEditor()
{
}

void PlaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (Theme::bgMain);
    auto panelBounds = getLocalBounds().toFloat().reduced(15.0f);
    g.setColour(Theme::bgPanel);
    g.fillRoundedRectangle(panelBounds, 8.0f);
    g.setColour(Theme::panelOutline);
    g.drawRoundedRectangle(panelBounds, 8.0f, 1.5f);

    g.setFont(juce::Font(juce::FontOptions("Helvetica", 16.0f, juce::Font::bold)).withExtraKerningFactor(0.2f));
    g.setColour(Theme::textLight);
    g.drawText("P L A C E", 0, 25, getWidth(), 30, juce::Justification::centred, true);

    g.setFont(juce::Font(juce::FontOptions("Helvetica", 11.0f, juce::Font::plain)).withExtraKerningFactor(0.1f));
    g.setColour(Theme::dimPurple);
    g.drawText("by leve", getWidth() - 100, 30, 80, 20, juce::Justification::right, true);
    g.drawText("v1.0.3",  getWidth() - 100, getHeight() - 40, 80, 20, juce::Justification::right, true);

    g.drawText("P L A C E", mainKnob.getX(), mainKnob.getBottom() + 10, mainKnob.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("SIDE BASS", sideBassKnob.getX() - 20, sideBassKnob.getBottom() + 8, sideBassKnob.getWidth() + 40, 20, juce::Justification::centred, true);
}

void PlaceAudioProcessorEditor::resized()
{
    mainKnob.setBounds(getLocalBounds().withSizeKeepingCentre(150, 150).withY(70));
    sideBassKnob.setBounds(getLocalBounds().withSizeKeepingCentre(60, 60).withY(250));
}
