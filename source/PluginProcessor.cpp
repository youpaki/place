#include "PluginProcessor.h"
#include "PluginEditor.h"

PlaceAudioProcessor::PlaceAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                          .withInput ("Sidechain", juce::AudioChannelSet::stereo(), false)),
      apvts (*this, nullptr, "PARAMETERS",
             [this]()
             {
                 juce::AudioProcessorValueTreeState::ParameterLayout layout;
                 paramManager = std::make_unique<ParameterManager> (layout);
                 return layout;
             }())
{
}

void PlaceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    staticEQ.prepare (sampleRate, samplesPerBlock);
    dynamicEQ.prepare (sampleRate, samplesPerBlock);
    sideCompensation.prepare (sampleRate, samplesPerBlock);
    midHighPass.prepare (sampleRate, samplesPerBlock);
    sidechainDetector.prepare (sampleRate, samplesPerBlock);

    smoothedSize = paramManager->getSizeMaximizerNormalized();
    smoothedBass = paramManager->getSideBassRemoverNormalized();

    float smoothTimeSec = 0.01f;
    smoothCoeff = std::exp (-1.0f / (smoothTimeSec * static_cast<float> (sampleRate) / static_cast<float> (samplesPerBlock)));
}

void PlaceAudioProcessor::releaseResources() {}

bool PlaceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    auto mainIn = layouts.getMainInputChannelSet();
    auto mainOut = layouts.getMainOutputChannelSet();

    if (mainIn != juce::AudioChannelSet::stereo())
        return false;
    if (mainOut != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.inputBuses.size() > 1)
    {
        auto scIn = layouts.inputBuses.getReference (1);
        if (scIn != juce::AudioChannelSet::disabled() && scIn != juce::AudioChannelSet::stereo())
            return false;
    }

    return true;
}

void PlaceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (paramManager->getBypass())
        return;

    int numSamples = buffer.getNumSamples();

    float targetSize = paramManager->getSizeMaximizerNormalized();
    float targetBass = paramManager->getSideBassRemoverNormalized();
    updateSmoothing (targetSize, targetBass, numSamples);

    bool sizeActive = smoothedSize > 0.001f;
    bool bassActive = smoothedBass > 0.001f;

    if (!sizeActive && !bassActive)
        return;

    bool isVocalFollow = (paramManager->getMode() == ModeValues::VOCAL_FOLLOW_MODE);

    juce::AudioBuffer<float> scBuffer;
    bool hasSidechain = false;

    if (getBusCount (true) > 1)
    {
        auto* scBus = getBus (true, 1);
        if (scBus != nullptr && scBus->isEnabled())
        {
            scBuffer = scBus->getBusBuffer (buffer);
            hasSidechain = true;
        }
    }

    sidechainConnected = hasSidechain;

    if (isVocalFollow && hasSidechain && scBuffer.getNumChannels() >= 2)
    {
        sidechainDetector.analyze (scBuffer.getReadPointer (0),
                                   scBuffer.getReadPointer (1),
                                   numSamples);
    }

    midSideProcessor.encode (buffer, numSamples);

    auto* midChannel = buffer.getWritePointer (0);
    auto* sideChannel = buffer.getWritePointer (1);

    if (sizeActive)
    {
        if (isVocalFollow && hasSidechain)
        {
            dynamicEQ.process (midChannel, numSamples, smoothedSize, sidechainDetector);
        }
        else
        {
            staticEQ.process (midChannel, numSamples, smoothedSize);
        }

        sideCompensation.process (sideChannel, numSamples, smoothedSize);
    }

    if (bassActive)
    {
        float hpFreq = 20.0f + smoothedBass * 160.0f;
        midHighPass.process (sideChannel, numSamples, hpFreq);
    }

    midSideProcessor.decode (buffer, numSamples);
}

void PlaceAudioProcessor::updateSmoothing (float targetSize, float targetBass, int /*numSamples*/) noexcept
{
    smoothedSize = smoothCoeff * smoothedSize + (1.0f - smoothCoeff) * targetSize;
    smoothedBass = smoothCoeff * smoothedBass + (1.0f - smoothCoeff) * targetBass;
}

juce::AudioProcessorEditor* PlaceAudioProcessor::createEditor()
{
    return new PlaceAudioProcessorEditor (*this);
}

void PlaceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    auto xml = state.createXml();
    copyXmlToBinary (*xml, destData);
}

void PlaceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary (data, sizeInBytes);
    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PlaceAudioProcessor();
}
