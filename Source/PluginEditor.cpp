/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Test2AudioProcessorEditor::Test2AudioProcessorEditor (Test2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    playButton.setButtonText("Play");
    playButton.changeWidthToFitText(40);
    
    
   
    
    
    addAndMakeVisible(&playButton);
    playButton.onClick = [this] { playFunc(); };
    
}

Test2AudioProcessorEditor::~Test2AudioProcessorEditor()
{
}

//==============================================================================
void Test2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::black);
    

}

void Test2AudioProcessorEditor::resized()
{
    playButton.setBounds (40, 30, 20, 20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Test2AudioProcessorEditor::playFunc()
{
        playButton.setButtonText("LALALALA");
        if (Test2AudioProcessorEditor::playButtonState == 0)
        {
            Test2AudioProcessorEditor::playButtonState = 1;
        }
        else
            Test2AudioProcessorEditor::playButtonState = 0;
    
        //create new midi buffer
        //append play message to that new MIDI buffer
        //once buffer is updated, then the process block grabs that midi message
        
}
