/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 6 Feb 2023 2:27:11pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Track.h"

using namespace std;

class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(std::vector<Track>* trackTitles);
    ~DJAudioPlayer();

    /* juce::AudioSource Pure Virtual Function */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    /* Coursera Function */
    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void start();
    void stop();
    double getPositionRelative();
    
    /* Extra Function */
    juce::URL nextTrack();
    juce::URL previousTrack();
    int getSourceLength();
    void registerBasicFormats();
    bool checkSourceLoaded();
    
private:
    bool isSourceLoaded = false;
    int currentTrackID = -100;
    std::vector<Track>* trackTitles;
    
    // processing audio
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
