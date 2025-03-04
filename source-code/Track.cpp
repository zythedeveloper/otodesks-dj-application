/*
  ==============================================================================

    Track.cpp
    Created: 20 Feb 2023 8:39:09pm
    Author:  ZenYu

  ==============================================================================
*/

#include "Track.h"

Track::Track(string _songName, string _songLength, string _audioURL)
: songName(_songName),
  audioURL(_audioURL),
  songLength(_songLength){}
