/*
  ==============================================================================

    LibraryComponent.cpp
    Created: 2 Mar 2023 7:43:20pm
    Author:  ZenYu

  ==============================================================================
*/

#include "LibraryComponent.h"

LibraryComponent::LibraryComponent(DJAudioPlayer* _player, vector<Track>* _trackTitles)
: player(_player), trackTitles(_trackTitles)
{}

std::vector<Track> LibraryComponent::loadCSV()
{
    return csvComponent.readCSV();
}

void LibraryComponent::saveCSV()
{
    csvComponent.writeCSV(trackTitles);
}

void LibraryComponent::addTrack(const juce::StringArray& files)
{
    for (auto audioFile : files)
    {
        bool duplicate = false;
        
        // check for duplicated input audio file
        if (trackTitles->size())
        {
            for (auto existFile : *trackTitles)
            {
                std::string audioURL = audioFile.toStdString();
                if (audioURL == existFile.audioURL)
                {
                    duplicate = true;
                    break;
                }
            }
        }

        if (!duplicate)
        {
            player->registerBasicFormats();
            player->loadURL(juce::URL{ juce::File{ audioFile } });
            
            int sourceLength = player->getSourceLength();
            std::string songName = juce::File{audioFile}.getFileNameWithoutExtension().toStdString();
            std::string audioURL = audioFile.toStdString();
            std::string songLength = std::to_string( (int)floor(sourceLength / 60) ) + ":" + std::to_string( sourceLength % 60 );
            
            trackTitles->push_back( Track{songName, songLength, audioURL} );
        }
    }
}
