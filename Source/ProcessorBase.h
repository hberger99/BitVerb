/*
Use the ProcessorBase class as a base class for different processors.
You can use this one as a template!!!
*/
#include <JuceHeader.h>

#pragma once
class ProcessorBase  : public juce::AudioProcessor  // check juce::AudioProcessor 
{
public:
    //==============================================================================
    ProcessorBase()  {}
 
    //==============================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override {}
 
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override          { return nullptr; }
    bool hasEditor() const override                              { return false; }
 
    //==============================================================================
    const juce::String getName() const override                  { return {}; }
    bool acceptsMidi() const override                            { return false; }
    bool producesMidi() const override                           { return false; }
    double getTailLengthSeconds() const override                 { return 0; }
 
    //==============================================================================
    int getNumPrograms() override                                { return 0; }
    int getCurrentProgram() override                             { return 0; }
    void setCurrentProgram (int) override                        {}
    const juce::String getProgramName (int) override             { return {}; }
    void changeProgramName (int, const juce::String&) override   {}
 
    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override       {}
    void setStateInformation (const void*, int) override         {}
 
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};
