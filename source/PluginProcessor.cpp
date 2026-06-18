#include "PluginProcessor.h"
#include "PluginEditor.h"

// Helper function to initialize APVTS safely with your ParameterManager
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(std::unique_ptr<ParameterManager>& pm)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    pm = std::make_unique<ParameterManager>(layout);
    return layout;
}

//==============================================================================
PlaceAudioProcessor::PlaceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       // OPEN SIDECHAIN BUS FOR DAWS
                       .withInput  ("Sidechain", juce::AudioChannelSet::stereo(), false)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout(paramManager))
#endif
{
}

//==============================================================================
bool PlaceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true; // Allows Sidechain to be active or inactive
}

//==============================================================================
void PlaceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    staticEQ.prepare(sampleRate, samplesPerBlock);
    dynamicEQ.prepare(sampleRate, samplesPerBlock);
    sideCompensation.prepare(sampleRate, samplesPerBlock);
    midHighPass.prepare(sampleRate, samplesPerBlock);
    sidechainDetector.prepare(sampleRate, samplesPerBlock);

    currentLevel.store(0.0f);
    sidechainLevel.store(0.0f);
}

void PlaceAudioProcessor::releaseResources()
{
    staticEQ.reset();
    dynamicEQ.reset();
    sideCompensation.reset();
    midHighPass.reset();
    sidechainDetector.reset();
}

//==============================================================================
void PlaceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (paramManager->getBypass()) return;

    int numSamples = buffer.getNumSamples();

    // 1. Get beautifully formatted values from ParameterManager
    float placeAmount = paramManager->getSizeMaximizerNormalized();
    float bassCutFreq = paramManager->getHighPassFrequency();
    bool isVocalFollow = paramManager->getMode() == ModeValues::VOCAL_FOLLOW_MODE;
    int scSource = paramManager->getScSource();

    // 2. Encode to Mid/Side
    midSideProcessor.encode(buffer, numSamples);

    // 3. Sidechain Routing
    const float* scLeftChannel = nullptr;
    const float* scRightChannel = nullptr;

    if (scSource == 0 && getBusCount(true) > 1) { // 0 = EXT SC
        auto scBus = getBusBuffer(buffer, true, 1);
        if (scBus.getNumChannels() > 0) {
            scLeftChannel = scBus.getReadPointer(0);
            scRightChannel = scBus.getNumChannels() > 1 ? scBus.getReadPointer(1) : scLeftChannel;
            sidechainConnected = true;
        } else {
            // Fallback to internal if external is silent
            scLeftChannel = buffer.getReadPointer(0);
            scRightChannel = buffer.getNumChannels() > 1 ? buffer.getReadPointer(1) : scLeftChannel;
            sidechainConnected = false;
        }
    } else { // 1 = INT SC
        auto mainBus = getBusBuffer(buffer, true, 0);
        scLeftChannel = mainBus.getReadPointer(0);
        scRightChannel = mainBus.getNumChannels() > 1 ? mainBus.getReadPointer(1) : scLeftChannel;
        sidechainConnected = false;
    }

    // 4. Analyze SC & Push Level to UI Meter
    sidechainDetector.analyze(scLeftChannel, scRightChannel, numSamples);
    float maxScLevel = 0.0f;
    for (size_t i = 0; i < SidechainDetector::numBands; ++i) {
        maxScLevel = std::max(maxScLevel, sidechainDetector.getEnvelope(i));
    }
    sidechainLevel.store(maxScLevel); 

    // 5. Apply EQ
    auto* midChannel = buffer.getWritePointer(0);
    auto* sideChannel = buffer.getWritePointer(1);

    if (isVocalFollow) {
        dynamicEQ.process(midChannel, numSamples, placeAmount, sidechainDetector);
    } else {
        staticEQ.process(midChannel, numSamples, placeAmount);
    }

    sideCompensation.process(sideChannel, numSamples, placeAmount);
    midHighPass.process(sideChannel, numSamples, bassCutFreq);

    // 6. Decode back to L/R
    midSideProcessor.decode(buffer, numSamples);

    // Update Output Meter
    float rms = 0.0f;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        rms += buffer.getRMSLevel(channel, 0, numSamples);
    }
    currentLevel.store(rms / buffer.getNumChannels());
}

//==============================================================================
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