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
class Test2AudioProcessorEditor  : public juce::AudioProcessorEditor

{
public:
    

    Test2AudioProcessorEditor (Test2AudioProcessor&);
    ~Test2AudioProcessorEditor() override;

   bool playButtonState; //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Test2AudioProcessor& audioProcessor;
    void playFunc();
    juce::TextButton playButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Test2AudioProcessorEditor)
};
