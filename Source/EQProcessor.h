// Processor for EQ Section of the Plugin

#pragma once
#include "ProcessorBase.h"

class EQProcessor  : public ProcessorBase
{
public:
    EQProcessor() {
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // Set Filter types for lowpass and highpass
        lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // set to low shelf
        highpassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass); // set to high shelf
        
        // prepare to play for filter objects
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumInputChannels();
        // If numChannels is 0 set it to 2 by default so that the filters can still be made
        if(spec.numChannels == 0)
            spec.numChannels = 2; // temporary default setting.
        // initialize filters
        lowpassFilter.prepare(spec);
        highpassFilter.prepare(spec);
        // reset filtrs
        lowpassFilter.reset();
        highpassFilter.reset();
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = buffer.getNumChannels(); // get number of channels in buffer

        // Loop over Channels
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            //Create channel Read/Write buffers
            auto* channelRead = buffer.getReadPointer (channel); // Read Buffer aka In Buffer
            auto* channelData = buffer.getWritePointer (channel); // Write Buffer aka Out Buffer

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                //Process Audio Through Filters sample by sample
                // If bypass is on for both skip processsing
                if(bypass_low == 1 && bypass_high == 1){
                    return;
                }
                // If lowpass is bypassed then process the highpass filter only
                else if (bypass_low == 1 && bypass_high == 0){
                    channelData[sample] = powf(10,(gainHigh)/20) * highpassFilter.processSample(channel, channelRead[sample]) + channelData[sample];
                }
                // If highpass is bypassed then process the lowpass filter only
                else if (bypass_low == 0 && bypass_high == 1){
                    channelData[sample] = powf(10,(gainLow)/20) * lowpassFilter.processSample(channel, channelRead[sample]) + channelData[sample];
                }
                // Otherwise process both filters
                else{
                    channelData[sample] = (powf(10,(gainLow)/20) * lowpassFilter.processSample(channel, channelRead[sample])) + (powf(10,(gainHigh)/20) * highpassFilter.processSample(channel, channelRead[sample]));
                }
            }
        }
    }

    const juce::String getName() const override { return "EQ"; }
    
    void setParameters(float lowGain, float highGain, float lowFreq, float highFreq, float lowQ, float highQ, bool new_bypass_low, bool new_bypass_high){
        
        // Set Lowpass Filter Params
        gainLow = lowGain;
        lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // set to low shelf
        lowpassFilter.setCutoffFrequency(lowFreq); // set to the slider frequency
        lowpassFilter.setResonance(lowQ); // set to slider q value
        
        // Set Highpass Filter Params
        gainHigh = highGain;
        highpassFilter.setCutoffFrequency(highFreq); // set to slider frequency
        highpassFilter.setResonance(highQ); // set to slider q value
        
        // Set Bypass States
        bypass_low = new_bypass_low;
        bypass_high = new_bypass_high;
    }

private:
    juce::dsp::ProcessSpec spec; // spec object
    juce::dsp::StateVariableTPTFilter<float> lowpassFilter; // lowpass state variable filter object
    juce::dsp::StateVariableTPTFilter<float> highpassFilter; // highpass state variable filter object
    float gainLow; // gain for lowpass filter
    float gainHigh; // gain for highpass filter
    bool bypass_low; // bypass for lowpass filter
    bool bypass_high; // bypass for highpass filter
};
