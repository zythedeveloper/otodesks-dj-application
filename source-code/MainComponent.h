/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeskGUI.h"
#include "LibraryGUI.h"

class MainComponent   : public juce::AudioAppComponent
{
public:
    /* Constructor Function */
    MainComponent();
    ~MainComponent();

    /* juce::AudioSource Pure Virtual Function */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;
    
private:
    std::vector<Track> trackTitles;
    
    // waveformDisplay
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};
    // audioPlayer
    DJAudioPlayer player1{&trackTitles};
    DJAudioPlayer player2{&trackTitles};
    // individual GUI
    DeskGUI deckGUI1{&player1, formatManager, thumbCache};
    DeskGUI deckGUI2{&player2, formatManager, thumbCache};
    // mixAudioSource
    juce::MixerAudioSource mixerSource;
    // music library
    LibraryGUI libraryGUI{&trackTitles, &deckGUI1, &deckGUI2};
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
