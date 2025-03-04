/*
  ==============================================================================

    DeskGUI.h
    Created: 7 Feb 2023 12:19:21pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using namespace std;

class DeskGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::Timer
{
public:
    DeskGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeskGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadURL(juce::URL audioFile);
    
    /* juce::Button::Listener Pure Virtual Function */
    void buttonClicked (juce::Button *) override;

    /* juce::Slider::Listener Pure Virtual Function */
    void sliderValueChanged (juce::Slider *) override;
    
    /* juce::Timer Pure Virtual Function */
    void timerCallback() override;

private:
    std::string resourcePath;
    
    juce::ImageButton playButton{"playButton"};
    juce::ImageButton pauseButton{"pauseButton"};
    juce::ImageButton leftButton{"leftButton"};
    juce::ImageButton rightButton{"rightButton"};
    
    juce::ImageComponent volComponent{"volComponent"};
    juce::ImageComponent spdComponent{"spdComponent"};
    
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    juce::Label posLabel{"posLabel", "0:00"};
    juce::Label nameLabel{"nameLabel", "File not loaded..."};
    
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
    // reading an audio file
    juce::FileChooser fChooser{"Select File..."};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeskGUI)
};
