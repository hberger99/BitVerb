/*
  ==============================================================================

    BitCrush.h
    Created: 14 Dec 2023 11:49:47am
    Author:  Hanna Berger

  ==============================================================================
*/

#pragma once

class BitCrusher {
public:
    void set_sample_rate(const juce::dsp::ProcessSpec& spec)
    {
        sample_rate = spec.sampleRate; // set/update bit crush sample rate
        return;
    }
    void set_bit_depth(double new_bd)
    {
        bit_depth = new_bd; // set/update bit crush bit depth
    }
    void set_downsample_factor(int new_dsf)
    {
        downsample_factor = new_dsf; // set/update bit crush downsample rate
    }
    void set_qtype(bool new_qtype)
    {
        tread_or_rise = new_qtype; // set/update quantization type
    }
    // Bitcrushing using sample by sample processing
    float process_sample(float input_sample)
    {
        Q = 2 / powf(2,bit_depth); // Calculate number of quantization levels
        
        if (tread_or_rise == false) // If bool is false use Mid-Tread Quantization
        {
            // Mid Tread Quantization
            out_sample = (floor(input_sample/Q + 0.5)) * Q;
        }
        else // Otherwise use Mid-Rise Quantization
        {
            // Mid Rise Quantization
            out_sample = (floor(input_sample/Q) + 0.5) * Q;
        }
        
        // Counter for downsampling... If the count is divisible by the downsample factor then output that processed sample
        if (count%downsample_factor == 0)
        {
            current_sample = out_sample;
            count = 0;
        }

        count++; // increment the count by 1
        return current_sample;
    }
    
private:
    // Parameters for Bit Crushign
    double sample_rate; // sample rate of audio for bitcrushing
    double bit_depth;
    int downsample_factor;
    bool tread_or_rise; // midtread or midrise boolean
    
    float Q; // minimum quantization level
    float current_sample; // sample that actually comes out of the bitcrusher
    float out_sample;   // output of quantization that is checked with counter
    int count = 0; // count for skipping samples
    
};





