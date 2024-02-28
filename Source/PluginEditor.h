/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Berger_Hanna_BitVerbAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::ToggleButton::Listener
{
public:
    Berger_Hanna_BitVerbAudioProcessorEditor (Berger_Hanna_BitVerbAudioProcessor&);
    ~Berger_Hanna_BitVerbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Berger_Hanna_BitVerbAudioProcessor& audioProcessor;
    
    // General Plugin Declarations
    juce::Label title;
    juce::Label label_mix;
    juce::Slider mix_knob; // wet/dry knob
    std::unique_ptr<SliderAttachment> mix_attach;
    
    // *** Reverb Variables *** //
    juce::Label label_rv_section;
    // Room Size
    juce::Label label_room_size;
    juce::Slider room_size;
    std::unique_ptr<SliderAttachment> room_size_attach;
    // Damping
    juce::Label label_damping;
    juce::Slider damping;
    std::unique_ptr<SliderAttachment> damping_attach;
    // Stereo Width
    juce::Label label_width;
    juce::Slider stereo_width;
    std::unique_ptr<SliderAttachment> width_attach;
    // Bypass Button
    juce::TextButton rvbypass;
    std::unique_ptr<ButtonAttachment> rvbypass_attach;
    
    // *** Bitcrush Variables *** //
    juce::Label label_bc_section;
    // Bit Depth
    juce::Label label_bit_depth;
    juce::Slider bit_depth;
    std::unique_ptr<SliderAttachment> bit_depth_attach;
    // Downsample Factor
    juce::Label label_down_samp;
    juce::Slider down_samp;
    std::unique_ptr<SliderAttachment> down_samp_attach;
    // MidTread/MidRise
    juce::Label label_quant_type;
    juce::TextButton quant_type;
    std::unique_ptr<ButtonAttachment> quant_type_attach;
    // Bypass Button
    juce::TextButton bcbypass;
    std::unique_ptr<ButtonAttachment> bcbypass_attach;
    
    // *** EQ Variables *** //
    juce::Label label_eq_section;
    // Lowpass Filter
    juce::Label low_label;
    juce::Label label_low_gain;
    juce::Slider low_gain;
    std::unique_ptr<SliderAttachment> low_gain_attach;
    juce::Label label_low_freq;
    juce::Slider low_freq;
    std::unique_ptr<SliderAttachment> low_freq_attach;
    juce::Label label_low_q;
    juce::Slider low_q;
    std::unique_ptr<SliderAttachment> low_q_attach;
    // Highpass Filter
    juce::Label high_label;
    juce::Label label_high_gain;
    juce::Slider high_gain;
    std::unique_ptr<SliderAttachment> high_gain_attach;
    juce::Label label_high_freq;
    juce::Slider high_freq;
    std::unique_ptr<SliderAttachment> high_freq_attach;
    juce::Label label_high_q;
    juce::Slider high_q;
    std::unique_ptr<SliderAttachment> high_q_attach;
    // Lowpass bypass button
    juce::TextButton eqbypass;
    std::unique_ptr<ButtonAttachment> eqbypass_attach;
    // Highpass bypass button
    juce::TextButton eqbypass2;
    std::unique_ptr<ButtonAttachment> eqbypass_attach2;

    // Fonts!!!
    juce::Font pixeloid;
    juce::Font pixeloid_bold;
    
    // Background Image
    juce::Image background_image;
    
    void buttonClicked (juce::Button* button) override; // add listener to quantization type button to change button text

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Berger_Hanna_BitVerbAudioProcessorEditor)
};
