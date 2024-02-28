/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// Include header file for BitCrush object
#include "BitCrush.h"
// Include Processors for Effects
#include "ReverbProcessor.h"
#include "BitcrushProcessor.h"
#include "EQProcessor.h"

// define parameter IDS to make it easier for referencing them on the Value State Tree
#define MIXID "mix"
// Reverb
#define ROOMSIZEID "roomSize"
#define DAMPINGID "damping"
#define WIDTHID "width"
#define RVBYPASSID "bypassRV"
// Bitcrushing
#define BITDEPTHID "bitDepth"
#define DOWNSAMPFACID "downsampleFactor"
#define OVERDRIVEID "overdrive"
#define QTYPEID "qtype"
#define BCBYPASSID "bypassBC"
// EQ
#define LOWGAINID "lowGain"
#define LOWRESID "lowRes"
#define LOWFREQID "lowFreq"
#define HIGHGAINID "highGain"
#define HIGHRESID "highRes"
#define HIGHFREQID "highFreq"
#define EQ1BYPASSID "bypassLPEQ"
#define EQ2BYPASSID "bypassHPEQ"

//==============================================================================
/**
*/
class Berger_Hanna_BitVerbAudioProcessor  : public juce::AudioProcessor, private juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Berger_Hanna_BitVerbAudioProcessor();
    ~Berger_Hanna_BitVerbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // Create a Value Tree Object
    juce::AudioProcessorValueTreeState parameters;

private:
    
    // order of processors for processor chain
    enum {
        revIndex,
        bitCrushIndex,
        eqIndex
    };
    
    // Variable for the wet/dry mix
    std::atomic<float> * mixParameter = nullptr;
    
    // Buffer object to copy the buffer before processing for the dry/wet mix
    juce::AudioBuffer<float> savedBuffer;
    
    // create processor chain object with the 3 audio effect processors in order
    juce::dsp::ProcessorChain<ReverbProcessor, BitCrushProcessor, EQProcessor> processorChain;
    
    // callback function from the juce::AudioProcessorValueTreeState::Listener
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Berger_Hanna_BitVerbAudioProcessor)
};
