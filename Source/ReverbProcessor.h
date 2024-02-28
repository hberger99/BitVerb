// Processor for Reverb Section of the plugin

#pragma once
#include "ProcessorBase.h"

class ReverbProcessor  : public ProcessorBase
{
public:
    ReverbProcessor() {
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // set up spec sample rate, max block size, and number of channels for reverb
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumInputChannels();
        
        // Prepare the reverb object for processing (default Juce DSP Reverb and Parameters)
        reverb.prepare(spec);
        reverb.setEnabled(true);
        reverb.reset();
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::ScopedNoDenormals noDenormals;
        
        // If bypass is on then jump out of the loop and don't process audio
        if(bypass == 1){
            return;
        }
        // Otherwise process reverb
        else{
            // Create a ProcessContextReplacing object
            juce::dsp::AudioBlock<float> audioBlock(buffer);
            juce::dsp::ProcessContextReplacing<float> context(audioBlock);
            // Process audio through the reverb using context replacing object
            reverb.process(context);
        }
    }

    const juce::String getName() const override { return "Reverb"; }
    
    void setParameters(float roomSizeParameter,float dampingParameter, float widthParameter, bool new_bypass){
        
        // set reverb parameters and assign them to the reverb object
        revParams.width = widthParameter;
        revParams.roomSize = roomSizeParameter;
        revParams.damping = dampingParameter;
        
        // These are hard coded since the user is unable to manipulate them
        revParams.wetLevel = 1.0;
        revParams.dryLevel = 0.1;
        revParams.freezeMode = 0.115;
        reverb.setParameters(revParams);
        
        // reset bypass value
        bypass = new_bypass;
    }

private:
    juce::dsp::ProcessSpec spec; // Spec for processor
    juce::dsp::Reverb reverb; // Reverb object
    juce::dsp::Reverb::Parameters revParams; // Reverb parameters object
    bool bypass; // bypass parameter
};
