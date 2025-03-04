/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 6 Feb 2023 2:27:11pm
    Author:  ZenYu

  ==============================================================================
*/

#include "DJAudioPlayer.h"

/* Constructor Function */
DJAudioPlayer::DJAudioPlayer(std::vector<Track>* _trackTitles)
: trackTitles(_trackTitles)
{}

DJAudioPlayer::~DJAudioPlayer(){}



/* juce::AudioSource Pure Virtual Function */
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // register all basic audio formats if not yet done
    if(!formatManager.getNumKnownFormats()) formatManager.registerBasicFormats();
    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}



/* Customised Function */
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        if(!isSourceLoaded) transportSource.setGain(0.0); // volume equals to zero
        readerSource.reset(newSource.release());
        isSourceLoaded = true;
    }
    
    if (isSourceLoaded)
    {
        // search for the loaded track's id
        for (unsigned int i=0; i<trackTitles->size(); ++i)
        {
            if (audioURL == juce::URL{ juce::File{ (*trackTitles)[i].audioURL }})
            {
                currentTrackID = i;
                break;
            }
        }
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
        std::cout << "DJAudioPlayer::setGain gain shoule between 0 and 1." << std::endl;
    else
        transportSource.setGain(gain);
    
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.0 || ratio > 10.0)
        std::cout << "DJAudioPlayer::setSpeed ratio shoule between 0 and 10." << std::endl;
    else
        resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
        std::cout << "DJAudioPlayer::setPositionRelative pos shoule between 0 and 1." << std::endl;
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}


/* Extra Function */
juce::URL DJAudioPlayer::nextTrack()
{
    juce::URL audioFile{""};
    ++currentTrackID;
    if (currentTrackID > -1)
    {
        // check if exceeded maximum size then reassign to the first track
        if (currentTrackID > trackTitles->size() - 1) currentTrackID = 0;
        audioFile = juce::URL{ juce::File{ (*trackTitles)[currentTrackID].audioURL } };
    }
    return audioFile;
}

juce::URL DJAudioPlayer::previousTrack()
{
    juce::URL audioFile{""};
    --currentTrackID;
    if (currentTrackID >= -1)
    {
        // check if exceeded minimum size then reassign to the last track
        if (currentTrackID == -1) currentTrackID = (int)trackTitles->size() - 1;
        audioFile = juce::URL{ juce::File{ (*trackTitles)[currentTrackID].audioURL } };
    }
    return audioFile;
}

int DJAudioPlayer::getSourceLength()
{
    if(isSourceLoaded) return transportSource.getLengthInSeconds();
    else return -1;
}

void DJAudioPlayer::registerBasicFormats()
{
    if(!formatManager.getNumKnownFormats()) formatManager.registerBasicFormats();
}

bool DJAudioPlayer::checkSourceLoaded()
{
    return isSourceLoaded;
}
