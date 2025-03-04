/*
  ==============================================================================

    Track.h
    Created: 20 Feb 2023 8:39:09pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once
#include <string>

using namespace std;

class Track
{
public:
    Track(string _songName, string _songLength, string _audioURL);
    
    string songName;
    string audioURL;
    string songLength;
};
