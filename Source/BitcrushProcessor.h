// Processor for BitCrush Section of Plugin

#pragma once
#include "BitCrush.h"
#include "ProcessorBase.h"

class BitCrushProcessor  : public ProcessorBase
{
public:
    BitCrushProcessor() {

    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // Prepare spec object
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumInputChannels();
        // use the spec object to set the sample rate of the bit crush
        bitcrush.set_sample_rate(spec);
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = buffer.getNumChannels();

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // Channel Read and Write Pointers
            auto* channelRead = buffer.getReadPointer (channel); // Read Buffer aka In Buffer
            auto* channelData = buffer.getWritePointer (channel); // Write Buffer aka Out Buffer
            // Loop through the channel and buffers sample by sample
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
                if(bypass == 1){ // if bypass is true then don't process bit crushing
                    return;
                }
                else{
                    // Process Audio Through Bitcrusher (see BitCrush.h)
                    channelData[sample] = bitcrush.process_sample(channelRead[sample]);
                }
            }
        }
    }

    const juce::String getName() const override { return "BitCrush"; }
    
    void setParameters(float bitDepthParameter, float downSampFactorParameter, bool qTypeParameter, bool new_bypass){
        // assign each of the parameters for the bitcrushing
        bitcrush.set_bit_depth(bitDepthParameter);
        bitcrush.set_downsample_factor(downSampFactorParameter);
        bitcrush.set_qtype(qTypeParameter);
        // reset bypass value
        bypass = new_bypass;
    }

private:
    // Spec Object
    juce::dsp::ProcessSpec spec;
    // Bitcrusher Object
    BitCrusher bitcrush;
    // bypass bool for bit crush module
    bool bypass;
    
};
