/*
  ==============================================================================

    LibraryComponent.h
    Created: 2 Mar 2023 7:43:20pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include <string>
#include <cmath>
#include "Track.h"
#include "DJAudioPlayer.h"
#include "CSVComponent.h"

class LibraryComponent
{
public:
    LibraryComponent(DJAudioPlayer* player, vector<Track>* trackTitles);
    
    std::vector<Track> loadCSV();
    void saveCSV();
    void addTrack(const juce::StringArray& files);
    
private:
    DJAudioPlayer* player;
    std::vector<Track>* trackTitles;
    CSVComponent csvComponent{"musicList.csv"};
};
