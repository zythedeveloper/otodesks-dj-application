/*
  ==============================================================================

    CSVComponent.h
    Created: 24 Feb 2023 10:14:50am
    Author:  ZenYu

  ==============================================================================
*/

#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <JuceHeader.h>
#include "Track.h"

using namespace std;

class CSVComponent
{
  public:
      CSVComponent(std::string csvPath);
      std::vector<Track> readCSV();
      void writeCSV(std::vector<Track>* trackTitles);
      
  private:
      std::string csvPath;
      void createCSV();
      std::vector<string> tokenise(std::string csvLine, char separator);
};
