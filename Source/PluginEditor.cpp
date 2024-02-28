/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Berger_Hanna_BitVerbAudioProcessorEditor::Berger_Hanna_BitVerbAudioProcessorEditor (Berger_Hanna_BitVerbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    auto width = 1000.0;
    auto height = 700.0;
    setSize (width, height);
    setResizable(false, false);  // set plugin to not be resizable
    
    // Import the Fonts
    pixeloid = juce::Typeface::createSystemTypefaceFor(BinaryData::PixeloidSansmLxMm_ttf, BinaryData::PixeloidSansmLxMm_ttfSize);
    pixeloid_bold = juce::Typeface::createSystemTypefaceFor(BinaryData::PixeloidSansBoldPKnYd_ttf, BinaryData::PixeloidSansBoldPKnYd_ttfSize);
    
    // Import Image
    background_image = juce::ImageCache::getFromMemory(BinaryData::Mountains_jpg, BinaryData::Mountains_jpgSize);
    
    // ***** Now set up each of the objects on the GUI!!! ***** //
    // Main Title
    title.setText("Bit-Verb!", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::horizontallyCentred); // center the text
    title.setFont(juce::Font(pixeloid_bold).withHeight(56.0f)); // set custom font
    title.setColour(juce::Label::textColourId, juce::Colours::white); // set title color
    addAndMakeVisible(title);
    
    // Wet / Dry Knob
    mix_knob.setSliderStyle(juce::Slider::Rotary);
    mix_knob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, width * 0.2, height * 0.05);
    mix_knob.setTextBoxIsEditable(true);
    mix_knob.setRange(0.0, 100.0, 1.0); // wet/dry mix range
    mix_knob.setValue(50.0); // initial mix value
    mix_knob.setTextValueSuffix("%");
    mix_knob.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(mix_knob);
    mix_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, MIXID, mix_knob);
    
    // Wet / Dry Label
    label_mix.setText("Wet/Dry", juce::dontSendNotification);
    label_mix.setJustificationType(juce::Justification::horizontallyCentred);
    label_mix.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_mix);
    
    // *** Reverb Section *** //
    // Reverb Title
    label_rv_section.setText("Reverb", juce::dontSendNotification);
    label_rv_section.setJustificationType(juce::Justification::horizontallyCentred); // center the text
    label_rv_section.setFont(juce::Font(pixeloid_bold).withHeight(48.0f)); // set custom font
    label_rv_section.setColour(juce::Label::textColourId, juce::Colours::white); // set title color
    addAndMakeVisible(label_rv_section);
    
    // Bypass Button
    rvbypass.setButtonText("Reverb Bypass");
    rvbypass.setClickingTogglesState(true); // allow it to be toggleable
    rvbypass.setToggleState(false, juce::NotificationType::dontSendNotification); // set initial toggle state to on
    rvbypass.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    rvbypass.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    rvbypass.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    rvbypass.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(rvbypass);
    rvbypass_attach = (std::unique_ptr<ButtonAttachment>) new ButtonAttachment(audioProcessor.parameters, RVBYPASSID, rvbypass);
    
    // Room Size Label
    label_room_size.setText("Room Size", juce::dontSendNotification);
    label_room_size.setJustificationType(juce::Justification::centred);
    label_room_size.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_room_size);
 
    // Room Size Slider
    room_size.setSliderStyle(juce::Slider::Rotary);
    room_size.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    room_size.setRange(0.0, 1.0, 0.01);
    room_size.setValue(0.5);
    room_size.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(room_size);
    room_size_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, ROOMSIZEID, room_size);
    
    // Damping Label
    label_damping.setText("Damping", juce::dontSendNotification);
    label_damping.setJustificationType(juce::Justification::centred);
    label_damping.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_damping);
 
    // Damping Slider
    damping.setSliderStyle(juce::Slider::Rotary);
    damping.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    damping.setRange(0.0, 1.0, 0.01);
    damping.setValue(0.5);
    damping.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(damping);
    damping_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, DAMPINGID, damping);
    
    // Width Label
    label_width.setText("Stereo Width", juce::dontSendNotification);
    label_width.setJustificationType(juce::Justification::centred);
    label_width.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_width);
 
    // Width Slider
    stereo_width.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    stereo_width.setRange(0.0, 1.0, 0.01);
    stereo_width.setValue(0.5);
    stereo_width.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(stereo_width);
    width_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, WIDTHID, stereo_width);
    
    // *** BitCrush Section *** //
    // Bitcrush Title
    label_bc_section.setText("Bit Crush", juce::dontSendNotification);
    label_bc_section.setJustificationType(juce::Justification::horizontallyCentred); // center the text
    label_bc_section.setFont(juce::Font(pixeloid_bold).withHeight(48.0f)); // set custom font
    label_bc_section.setColour(juce::Label::textColourId, juce::Colours::white); // set title color
    addAndMakeVisible(label_bc_section);
    
    // Bypass Button
    bcbypass.setButtonText("Bit Crush Bypass");
    bcbypass.setClickingTogglesState(true); // allow it to be toggleable
    bcbypass.setToggleState(false, juce::NotificationType::dontSendNotification); // set initial toggle state to off
    bcbypass.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    bcbypass.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    bcbypass.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    bcbypass.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(bcbypass);
    bcbypass_attach = (std::unique_ptr<ButtonAttachment>) new ButtonAttachment(audioProcessor.parameters, BCBYPASSID, bcbypass);
    
    // Bit Depth Label
    label_bit_depth.setText("Bit Depth", juce::dontSendNotification);
    label_bit_depth.setJustificationType(juce::Justification::centred);
    label_bit_depth.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_bit_depth);
 
    // Bit Depth Slider
    bit_depth.setSliderStyle(juce::Slider::Rotary);
    bit_depth.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    bit_depth.setRange(0.0, 16.0, 1.0);
    bit_depth.setValue(8.0);
    bit_depth.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(bit_depth);
    bit_depth_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, BITDEPTHID, bit_depth);
    
    // Downsample Factor Label
    label_down_samp.setText("Downsample Factor", juce::dontSendNotification);
    label_down_samp.setJustificationType(juce::Justification::centred);
    label_down_samp.setFont(juce::Font(pixeloid).withHeight(20.0f)); // set custom font
    addAndMakeVisible(label_down_samp);
 
    // Downsample Factor Slider
    down_samp.setSliderStyle(juce::Slider::Rotary);
    down_samp.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    down_samp.setRange(1.0, 10.0, 1.0);
    down_samp.setValue(2.0);
    down_samp.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(down_samp);
    down_samp_attach = (std::unique_ptr<SliderAttachment>) new SliderAttachment(audioProcessor.parameters, DOWNSAMPFACID, down_samp);
    
    // Quant Type Label
    label_quant_type.setText("Quantization Type", juce::dontSendNotification);
    label_quant_type.setJustificationType(juce::Justification::centred);
    label_quant_type.setFont(juce::Font(pixeloid).withHeight(24.0f)); // set custom font
    addAndMakeVisible(label_quant_type);
    
    // Quant Type Button
    quant_type.setButtonText("Mid-Tread");
    quant_type.setClickingTogglesState(true); // allow it to be toggleable
    quant_type.setToggleState(false, juce::NotificationType::dontSendNotification); // set initial toggle state to off
    quant_type.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    quant_type.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    quant_type.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    quant_type.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(quant_type);
    //quant_type_attach = (std::unique_ptr<ButtonAttachment>) new ButtonAttachment(audioProcessor.parameters, QTYPEID, quant_type);
    quant_type.addListener(this); // add listener to use callback function to change the button text
    
    // *** EQ Section *** //
    // EQ Title
    label_eq_section.setText("EQ", juce::dontSendNotification);
    label_eq_section.setJustificationType(juce::Justification::horizontallyCentred); // center the text
    label_eq_section.setFont(juce::Font(pixeloid_bold).withHeight(48.0f)); // set custom font
    label_eq_section.setColour(juce::Label::textColourId, juce::Colours::white); // set title color
    addAndMakeVisible(label_eq_section);
    
    // EQ Buttons
    // Lowpass Button
    eqbypass.setButtonText("Low Shelf\nBypass");
    eqbypass.setClickingTogglesState(true); // allow it to be toggleable
    eqbypass.setToggleState(true, juce::NotificationType::dontSendNotification); // set initial toggle state to on
    eqbypass.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    eqbypass.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    eqbypass.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    eqbypass.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(eqbypass);
    eqbypass_attach = (std::unique_ptr<ButtonAttachment>) new ButtonAttachment(audioProcessor.parameters, EQ1BYPASSID, eqbypass);
    // Highpass Button
    eqbypass2.setButtonText("High Shelf\nBypass");
    eqbypass2.setClickingTogglesState(true); // allow it to be toggleable
    eqbypass2.setToggleState(true, juce::NotificationType::dontSendNotification); // set initial toggle state to on
    eqbypass2.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    eqbypass2.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    eqbypass2.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    eqbypass2.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(eqbypass2);
    eqbypass_attach2 = (std::unique_ptr<ButtonAttachment>) new ButtonAttachment(audioProcessor.parameters, EQ2BYPASSID, eqbypass2);
    
    /* Low Shelf Section */
    // Low Section Freq Label
    label_low_freq.setText("Low Shelf Frequency", juce::dontSendNotification);
    label_low_freq.setJustificationType(juce::Justification::horizontallyCentred);
    label_low_freq.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_low_freq.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_low_freq);
    
    // Low Freq Slider
    low_freq.setSliderStyle(juce::Slider::Rotary);
    low_freq.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    low_freq.setTextBoxIsEditable(true);
    low_freq.setTextValueSuffix(" Hz");
    low_freq.setNumDecimalPlacesToDisplay(2);
    low_freq.setRange(10.0, 1000.0, 1.0);
    low_freq.setValue(500.0);
    low_freq.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(low_freq);
    low_freq_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, LOWFREQID, low_freq);
    
    // Low Section Gain Label
    label_low_gain.setText("Low Shelf Gain", juce::dontSendNotification);
    label_low_gain.setJustificationType(juce::Justification::horizontallyCentred);
    label_low_gain.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_low_gain.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_low_gain);
    
    // Low Gain Slider
    low_gain.setSliderStyle(juce::Slider::Rotary);
    low_gain.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    low_gain.setTextBoxIsEditable(true);
    low_gain.setTextValueSuffix(" dB");
    low_gain.setNumDecimalPlacesToDisplay(2);
    low_gain.setRange(-20.0, 20.0, 1.0);
    low_gain.setValue(0.0);
    low_gain.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(low_gain);
    low_gain_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, LOWGAINID, low_gain);
    
    // Low Section Q Label
    label_low_q.setText("Low Shelf Q", juce::dontSendNotification);
    label_low_q.setJustificationType(juce::Justification::horizontallyCentred);
    label_low_q.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_low_q.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_low_q);
    
    // Low Resonance (Q) Slider
    low_q.setSliderStyle(juce::Slider::Rotary);
    low_q.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    low_q.setTextBoxIsEditable(true);
    low_q.setNumDecimalPlacesToDisplay(2);
    low_q.setRange(0.0, 10.0, 0.01);
    low_q.setValue(1.0);
    low_q.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(low_q);
    low_q_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, LOWRESID, low_q);
    
    /* High Shelf Section */
    // High Section Freq Label
    label_high_freq.setText("High Shelf Frequency", juce::dontSendNotification);
    label_high_freq.setJustificationType(juce::Justification::horizontallyCentred);
    label_high_freq.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_high_freq.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_high_freq);
    
    // High Freq Slider
    high_freq.setSliderStyle(juce::Slider::Rotary);
    high_freq.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    high_freq.setTextBoxIsEditable(true);
    high_freq.setNumDecimalPlacesToDisplay(2);
    high_freq.setRange(2000.0, 10000.0, 1.0);
    high_freq.setValue(500.0);
    high_freq.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(high_freq);
    high_freq_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, HIGHFREQID, high_freq);
    
    // High Section Gain Label
    label_high_gain.setText("High Shelf Gain", juce::dontSendNotification);
    label_high_gain.setJustificationType(juce::Justification::horizontallyCentred);
    label_high_gain.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_high_gain.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_high_gain);
    
    // High Gain Slider
    high_gain.setSliderStyle(juce::Slider::Rotary);
    high_gain.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    high_gain.setTextBoxIsEditable(true);
    high_gain.setTextValueSuffix(" dB");
    high_gain.setNumDecimalPlacesToDisplay(2);
    high_gain.setRange(-20.0, 20.0, 1.0);
    high_gain.setValue(0.0);
    high_gain.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(high_gain);
    high_gain_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, HIGHGAINID, high_gain);
    
    // High Section Q Label
    label_high_q.setText("High Shelf Q", juce::dontSendNotification);
    label_high_q.setJustificationType(juce::Justification::horizontallyCentred);
    label_high_q.setFont(juce::Font(pixeloid).withHeight(18.0f)); // set custom font
    label_high_q.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label_high_q);
    
    // High Resonance (Q) Slider
    high_q.setSliderStyle(juce::Slider::Rotary);
    high_q.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);
    high_q.setTextBoxIsEditable(true);
    high_q.setNumDecimalPlacesToDisplay(2);
    high_q.setRange(0.0, 10.0, 0.01);
    high_q.setValue(1.0);
    high_q.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    addAndMakeVisible(high_q);
    high_q_attach = (std::unique_ptr<SliderAttachment>) new  SliderAttachment(audioProcessor.parameters, HIGHRESID, high_q);
}

Berger_Hanna_BitVerbAudioProcessorEditor::~Berger_Hanna_BitVerbAudioProcessorEditor()
{
}

//==============================================================================
void Berger_Hanna_BitVerbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Basic Dimensions for the GUI
    auto border = 10;
    auto area = getLocalBounds().reduced(border);
    auto width = area.getWidth();
    auto height = area.getHeight();
    auto section_ratioW = 1.0/3.0;
    
    // Set Backgrond Image
    g.drawImage(background_image, 0, 0, getWidth(), getHeight(), 0, 0, background_image.getWidth(), background_image.getHeight());
    
    // Create a custom transparent color for the module sections
    juce::Colour myTransparentColor(0x60400080);
    
    // Basic Sectioning
    auto title_section = area.removeFromTop(1.0/4.0 * height);
    auto title_section_height = title_section.getHeight();
    title.setBounds(title_section.reduced(20));
    auto processing_section = area.removeFromTop(4.0/5.0 * height);
    auto process_sect_height = processing_section.getHeight();
    auto reverb_section = processing_section.removeFromLeft(section_ratioW*width).reduced(7.5);
    g.setColour(myTransparentColor);
    g.fillRect(reverb_section.reduced(5));
    auto bitcrush_section = processing_section.removeFromLeft(section_ratioW*width).reduced(7.5);
    g.setColour(myTransparentColor);
    g.fillRect(bitcrush_section.reduced(5));
    auto eq_section = processing_section.removeFromLeft(section_ratioW*width).reduced(7.5);
    g.setColour(myTransparentColor);
    g.fillRect(eq_section.reduced(5));
    
    // *** Other Stuff in the Top Section *** //
    // Section to eventually be home to presets
    auto preset_section = title_section.removeFromLeft(section_ratioW*width);
    // Placeholder
    auto title_section_placeholder = title_section.removeFromLeft(section_ratioW*width);
    // Wet Dry Knob
    auto mix_section = title_section.removeFromLeft(section_ratioW*width);
    mix_knob.setBounds(mix_section.reduced(35));
    auto mix_label_section = mix_section.removeFromTop(1.0/4.0*title_section_height);
    label_mix.setBounds(mix_label_section.reduced(5));
    
    // *** Reverb Section *** //
    // ratios for parameter sectioning
    auto ratioW = 1.0/3.0;
    auto ratioH = 1.0/5.0;
    // Reverb Title
    auto rv_title_section = reverb_section.removeFromTop(ratioH*process_sect_height);
    label_rv_section.setBounds(rv_title_section.reduced(10));
    // Reverb Bypass
    auto rvbypass_sect = reverb_section.removeFromTop(ratioH*process_sect_height);
    rvbypass.setBounds(rvbypass_sect.reduced(25));
    // Room Size
    auto rvrow1 = reverb_section.removeFromTop(3.0/2.0*ratioH*process_sect_height);
    auto rvrowheight = rvrow1.getHeight();
    auto rs_area = rvrow1.removeFromLeft(0.5*ratioW*width);
    room_size.setBounds(rs_area.reduced(25));
    auto rs_label_area = rs_area.removeFromTop(1.0/4.0*rvrowheight);
    label_room_size.setBounds(rs_label_area.reduced(5));
    // Damping
    auto damp_area = rvrow1.removeFromLeft(0.5*ratioW*width);
    damping.setBounds(damp_area.reduced(25));
    auto damp_label_area = damp_area.removeFromTop(1.0/4.0*rvrowheight);
    label_damping.setBounds(damp_label_area.reduced(5));
    // Stereo Width
    auto width_area = reverb_section.removeFromTop(3.0/2.0*ratioH*process_sect_height);
    stereo_width.setBounds(width_area.reduced(15));
    auto width_label_area = width_area.removeFromTop(1.0/4.0*rvrowheight);
    label_width.setBounds(width_label_area.reduced(5));
    
    // *** BitCrush Section *** //
    // BitCrush Title
    auto bc_title_section = bitcrush_section.removeFromTop(ratioH*process_sect_height);
    label_bc_section.setBounds(bc_title_section.reduced(10));
    // BitCrush Bypass
    auto bcbypass_sect = bitcrush_section.removeFromTop(ratioH*process_sect_height);
    bcbypass.setBounds(bcbypass_sect.reduced(25));
    auto bcrow1 = bitcrush_section.removeFromTop(3.0/2.0*ratioH*process_sect_height);
    auto bcrowheight = bcrow1.getHeight();
    // Bit Depth
    auto bd_area = bcrow1.removeFromLeft(0.5*ratioW*width);
    bit_depth.setBounds(bd_area.reduced(25));
    auto bd_label_area = bd_area.removeFromTop(1.0/4.0*bcrowheight);
    label_bit_depth.setBounds(bd_label_area.reduced(5));
    // Downsample
    auto ds_area = bcrow1.removeFromLeft(0.5*ratioW*width);
    down_samp.setBounds(ds_area.reduced(25));
    auto ds_label_area = ds_area.removeFromTop(1.0/4.0*bcrowheight);
    label_down_samp.setBounds(ds_label_area);
    // Quantization Type
    auto qtype_area = bitcrush_section.removeFromTop(3.0/2.0*ratioH*process_sect_height);
    quant_type.setBounds(qtype_area.reduced(50));
    auto qtype_label_area = qtype_area.removeFromTop(1.0/4.0*bcrowheight);
    label_quant_type.setBounds(qtype_label_area.reduced(5));
    
    // *** EQ Section *** //
    // EQ Title
    auto eq_title_section = eq_section.removeFromTop(ratioH*process_sect_height);
    label_eq_section.setBounds(eq_title_section.reduced(10));
    // EQ Bypass
    auto eqbypass_sect = eq_section.removeFromTop(ratioH*process_sect_height);
    auto low_bypass_sect = eqbypass_sect.removeFromLeft(0.5*ratioW*width);
    eqbypass.setBounds(low_bypass_sect.reduced(25));
    auto high_bypass_sect = eqbypass_sect.removeFromLeft(0.5*ratioW*width);
    eqbypass2.setBounds(high_bypass_sect.reduced(25));
    
    // Rows for EQ Sliders
    auto eqrow1 = eq_section.removeFromTop(ratioH*process_sect_height);
    auto eqrowheight = eqrow1.getHeight();
    // Low Cutoff Frequency
    auto lf_area = eqrow1.removeFromLeft(0.5*ratioW*width);
    low_freq.setBounds(lf_area.reduced(15));
    auto lf_label_area = lf_area.removeFromTop(1.0/4.0*eqrowheight);
    label_low_freq.setBounds(lf_label_area.reduced(2));

    // High Cutoff Frequency
    auto hf_area = eqrow1.removeFromLeft(0.5*ratioW*width);
    high_freq.setBounds(hf_area.reduced(15));
    auto hf_label_area = hf_area.removeFromTop(1.0/4.0*eqrowheight);
    label_high_freq.setBounds(hf_label_area.reduced(2));
    
    auto eqrow2 = eq_section.removeFromTop(ratioH*process_sect_height);
    // Low Gain
    auto lg_area = eqrow2.removeFromLeft(0.5*ratioW*width);
    low_gain.setBounds(lg_area.reduced(15));
    auto lg_label_area = lg_area.removeFromTop(1.0/4.0*eqrowheight);
    label_low_gain.setBounds(lg_label_area.reduced(2));
    // High Gain
    auto hg_area = eqrow2.removeFromLeft(0.5*ratioW*width);
    high_gain.setBounds(hg_area.reduced(15));
    auto hg_label_area = hg_area.removeFromTop(1.0/4.0*eqrowheight);
    label_high_gain.setBounds(hg_label_area.reduced(2));

    auto eqrow3 = eq_section.removeFromTop(ratioH*process_sect_height);
    // Low Resonance
    auto lq_area = eqrow3.removeFromLeft(0.5*ratioW*width);
    low_q.setBounds(lq_area.reduced(15));
    auto lq_label_area = lq_area.removeFromTop(1.0/4.0*eqrowheight);
    label_low_q.setBounds(lq_label_area.reduced(2));
    // High Resonance
    auto hq_area = eqrow3.removeFromLeft(0.5*ratioW*width);
    high_q.setBounds(hq_area.reduced(15));
    auto hq_label_area = hq_area.removeFromTop(1.0/4.0*eqrowheight);
    label_high_q.setBounds(hq_label_area.reduced(2));
}

void Berger_Hanna_BitVerbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // Laid everything out in paint since it is not resizable
}

// Callback function for buttons specifically used for the Quantizer Type Button
void Berger_Hanna_BitVerbAudioProcessorEditor::buttonClicked (juce::Button* button){
    if (button == &quant_type) {
        if (quant_type.getToggleState() == true) { // if button turned on then button text is Mid-Rise
            quant_type.setButtonText("Mid-Rise");
        }
        else{
            quant_type.setButtonText("Mid-Tread"); // if the button is off then set button text to Mid-Tread
        }
    }
}
