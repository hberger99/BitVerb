/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Berger_Hanna_BitVerbAudioProcessor::Berger_Hanna_BitVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
,parameters(*this,         // Define all the parameters that are part of the value state tree!!!
             nullptr,juce::Identifier("Parameters"),{
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(ROOMSIZEID, 1),"roomSize", 0.0,1.0,0.5),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(DAMPINGID, 1),"damping", 0.0,1.0,0.5),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(WIDTHID, 1),"width", 0.0,1.0,0.5),
    std::make_unique<juce::AudioParameterBool> (
                    juce::ParameterID(RVBYPASSID, 2),"reverb bypass", false),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(BITDEPTHID, 1),"bitDepth", 0.0,16.0,8.0),
    std::make_unique<juce::AudioParameterInt> (
                    juce::ParameterID(DOWNSAMPFACID, 1),"downsampleFactor", 1.0,10.0,2.0),
    std::make_unique<juce::AudioParameterBool> (
                    juce::ParameterID(QTYPEID, 2),"qtype", false),
    std::make_unique<juce::AudioParameterBool> (
                    juce::ParameterID(BCBYPASSID, 2),"bitcrush bypass", false),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(LOWGAINID, 1),"lowshelf gain",-20.0,20.0,0.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(LOWFREQID, 1),"lowshelffreq",10.0,1000.0,500.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(LOWRESID, 1),"lowshelf q",0.0,10.0,1.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(HIGHGAINID, 1),"highshelf gain",-20.0,20.0,0.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(HIGHFREQID, 1),"highshelf freq",2000.0,10000.0,5000.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(HIGHRESID, 1),"highshelf q",0.0,10.0,1.0),
    std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID(MIXID, 1),"wet/dry mix",0.0,100.0,50.0),
    std::make_unique<juce::AudioParameterBool> (
                    juce::ParameterID(EQ1BYPASSID, 2),"lowpass bypass", true),
    std::make_unique<juce::AudioParameterBool> (
                    juce::ParameterID(EQ2BYPASSID, 2),"highpass bypass", true)
        }
             )
#endif
{
    // assign wet/dry value to the mixParameter pointer
    mixParameter = parameters.getRawParameterValue(MIXID);
    
    // Add listeners to each of the parameters on the value state tree
    parameters.addParameterListener(ROOMSIZEID, this);
    parameters.addParameterListener(DAMPINGID, this);
    parameters.addParameterListener(WIDTHID, this);
    parameters.addParameterListener(RVBYPASSID, this);
    parameters.addParameterListener(BITDEPTHID, this);
    parameters.addParameterListener(DOWNSAMPFACID, this);
    parameters.addParameterListener(QTYPEID, this);
    parameters.addParameterListener(BCBYPASSID, this);
    parameters.addParameterListener(LOWGAINID, this);
    parameters.addParameterListener(LOWFREQID, this);
    parameters.addParameterListener(LOWRESID, this);
    parameters.addParameterListener(HIGHGAINID, this);
    parameters.addParameterListener(HIGHFREQID, this);
    parameters.addParameterListener(HIGHRESID, this);
    parameters.addParameterListener(EQ1BYPASSID, this);
    parameters.addParameterListener(EQ2BYPASSID, this);
    parameters.addParameterListener(MIXID, this);
}

Berger_Hanna_BitVerbAudioProcessor::~Berger_Hanna_BitVerbAudioProcessor()
{
    DBG("PluginProcessor Destructor");
}

//==============================================================================
const juce::String Berger_Hanna_BitVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Berger_Hanna_BitVerbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Berger_Hanna_BitVerbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Berger_Hanna_BitVerbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Berger_Hanna_BitVerbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Berger_Hanna_BitVerbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Berger_Hanna_BitVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Berger_Hanna_BitVerbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Berger_Hanna_BitVerbAudioProcessor::getProgramName (int index)
{
    return {};
}

void Berger_Hanna_BitVerbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Berger_Hanna_BitVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Run Prepare to Play Function for each of the Processors
    processorChain.get<revIndex>().prepareToPlay(sampleRate, samplesPerBlock);
    processorChain.get<bitCrushIndex>().prepareToPlay(sampleRate, samplesPerBlock);
    processorChain.get<eqIndex>().prepareToPlay(sampleRate, samplesPerBlock);
    
    // Set the Parameters for each of the Processors
    processorChain.get<revIndex>().setParameters(*parameters.getRawParameterValue(ROOMSIZEID), *parameters.getRawParameterValue(DAMPINGID), *parameters.getRawParameterValue(WIDTHID), *parameters.getRawParameterValue(RVBYPASSID));
    processorChain.get<bitCrushIndex>().setParameters(*parameters.getRawParameterValue(BITDEPTHID), *parameters.getRawParameterValue(DOWNSAMPFACID), *parameters.getRawParameterValue(QTYPEID), *parameters.getRawParameterValue(BCBYPASSID));
    processorChain.get<eqIndex>().setParameters(*parameters.getRawParameterValue(LOWGAINID), *parameters.getRawParameterValue(HIGHGAINID), *parameters.getRawParameterValue(LOWFREQID), *parameters.getRawParameterValue(HIGHFREQID), *parameters.getRawParameterValue(LOWRESID), *parameters.getRawParameterValue(HIGHRESID), *parameters.getRawParameterValue(EQ1BYPASSID), *parameters.getRawParameterValue(EQ2BYPASSID));
}

void Berger_Hanna_BitVerbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Berger_Hanna_BitVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Berger_Hanna_BitVerbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    // save the input buffer for the wet/dry mix
    savedBuffer = buffer;
    
    // Call Processor Chain Objects in Order!!!
    processorChain.get<revIndex>().processBlock(buffer, midiMessages);
    processorChain.get<bitCrushIndex>().processBlock(buffer, midiMessages);
    processorChain.get<eqIndex>().processBlock(buffer, midiMessages);
    
    // Now apply the wet/dry mix!!!
    auto wet = parameters.getRawParameterValue(MIXID)->load();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        // Create Read/Write Pointers
        auto* originalData = savedBuffer.getReadPointer(channel);
        auto* processedData = buffer.getReadPointer(channel);
        auto* channelData = buffer.getWritePointer (channel); // Write Buffer aka Out Buffer
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Based off equation:  OUT = (processed_sample * wet) + (original_sample * dry)... dry = 1 - wet
            channelData[sample] = (processedData[sample] * (wet/ 100.0)) + (originalData[sample] * (1.0 - (wet/ 100.0)));
        }
    }
}

//==============================================================================
bool Berger_Hanna_BitVerbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Berger_Hanna_BitVerbAudioProcessor::createEditor()
{
    return new Berger_Hanna_BitVerbAudioProcessorEditor (*this);
}

//==============================================================================
void Berger_Hanna_BitVerbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();   // valueTree
    std::unique_ptr<juce::XmlElement> xml(state.createXml());   // a pointer
    copyXmlToBinary(*xml, destData);
}

void Berger_Hanna_BitVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    DBG("setStateInformation");
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    // replace an entire ValueTreeState (all the parameters)
    if(xmlState.get() != nullptr)
        if(xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml(*xmlState));
}

// callback function that is called when an audio parameter is changed
void Berger_Hanna_BitVerbAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue){
    // If Reverb parameter changed then update the Reverb Parameters!!!
    if(parameterID == ROOMSIZEID || parameterID == DAMPINGID || parameterID == WIDTHID || parameterID == RVBYPASSID){
        processorChain.get<revIndex>().setParameters(*parameters.getRawParameterValue(ROOMSIZEID), *parameters.getRawParameterValue(DAMPINGID), *parameters.getRawParameterValue(WIDTHID), *parameters.getRawParameterValue(RVBYPASSID));
    }
    // If BitCrush parameter changed then update the BitCrushing Parameters!!!
    else if (parameterID == BITDEPTHID || parameterID == DOWNSAMPFACID || parameterID == QTYPEID || parameterID == BCBYPASSID){
        processorChain.get<bitCrushIndex>().setParameters(*parameters.getRawParameterValue(BITDEPTHID), *parameters.getRawParameterValue(DOWNSAMPFACID), *parameters.getRawParameterValue(QTYPEID), *parameters.getRawParameterValue(BCBYPASSID));
    }
    // If EQ paramters changed then update the EQ Parameters!!!
    else if (parameterID == LOWGAINID || parameterID == LOWFREQID || parameterID == LOWRESID || parameterID == HIGHGAINID || parameterID == HIGHFREQID || parameterID == HIGHRESID || parameterID == EQ1BYPASSID || parameterID == EQ2BYPASSID){
        processorChain.get<eqIndex>().setParameters(*parameters.getRawParameterValue(LOWGAINID), *parameters.getRawParameterValue(HIGHGAINID), *parameters.getRawParameterValue(LOWFREQID), *parameters.getRawParameterValue(HIGHFREQID), *parameters.getRawParameterValue(LOWRESID), *parameters.getRawParameterValue(HIGHRESID), *parameters.getRawParameterValue(EQ1BYPASSID), *parameters.getRawParameterValue(EQ2BYPASSID));
    }
    // If the Mix Parameter is changed then update the pointer... I'm not sure if this is necessary but just in case
    else if (parameterID == MIXID){
        mixParameter = parameters.getRawParameterValue(MIXID);
    }
    else{
        DBG("Wrong parameter ID!!!");
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Berger_Hanna_BitVerbAudioProcessor();
}
