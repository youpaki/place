#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(std::unique_ptr<ParameterManager>& pm)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    pm = std::make_unique<ParameterManager>(layout);
    return layout;
}

PlaceAudioProcessor::PlaceAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       apvts (*this, nullptr, "Parameters", createParameterLayout(paramManager))
{
}

bool PlaceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void PlaceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    staticEQ.prepare(sampleRate, samplesPerBlock);
    sideHighPass.prepare(sampleRate, samplesPerBlock);
    currentLevel.store(0.0f);
}

void PlaceAudioProcessor::releaseResources()
{
    staticEQ.reset();
    sideHighPass.reset();
}

void PlaceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();

    float placeAmount = paramManager->getSizeMaximizerNormalized();
    float bassAmount = paramManager->getSideBassRemoverNormalized();

    bool needsProcessing = (placeAmount > 0.0f || bassAmount > 0.0f);

    if (!needsProcessing)
    {
        float rms = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            rms += buffer.getRMSLevel(ch, 0, numSamples);
        currentLevel.store(rms / buffer.getNumChannels());
        return;
    }

    midSideProcessor.encode(buffer, numSamples);

    auto* midChannel = buffer.getWritePointer(0);
    auto* sideChannel = buffer.getWritePointer(1);

    if (placeAmount > 0.0f)
        staticEQ.process(midChannel, numSamples, placeAmount);

    if (bassAmount > 0.0f)
        sideHighPass.process(sideChannel, numSamples, bassAmount);

    midSideProcessor.decode(buffer, numSamples);

    float rms = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        rms += buffer.getRMSLevel(ch, 0, numSamples);
    currentLevel.store(rms / buffer.getNumChannels());
}

juce::AudioProcessorEditor* PlaceAudioProcessor::createEditor() { return new PlaceAudioProcessorEditor (*this); }

void PlaceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PlaceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new PlaceAudioProcessor(); }
