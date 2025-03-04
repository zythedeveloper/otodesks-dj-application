/*
  ==============================================================================

    WaveformDisplay.h
    Created: 13 Feb 2023 4:37:51pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace std;

class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadURL(URL audioURL);
    
    /* juce::ChangeListener Pure Virtual Function */
    void changeListenerCallback (ChangeBroadcaster *source) override;
    
    void setPositionRelative(double pos);

private:
    double position;
    bool sourceLoaded;
    
    AudioThumbnail audioThumb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
