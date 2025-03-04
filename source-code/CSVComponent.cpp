/*
  ==============================================================================

    CSVComponent.cpp
    Created: 24 Feb 2023 10:14:50am
    Author:  ZenYu

  ==============================================================================
*/

#include "CSVComponent.h"

CSVComponent::CSVComponent(std::string _csvPath)
: csvPath(_csvPath)
{
    createCSV();
}

void CSVComponent::createCSV()
{
    // getting the parent path to Otodecks Application
    juce::String absJucePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getFullPathName();
    // concatenate the parent path with our csv path
    csvPath = absJucePath.toStdString() + "/OtoDesksResource/" + csvPath;
    std::cout << csvPath << std::endl;
    
    std::fstream myFile;
    if (!myFile.is_open()) myFile.open(csvPath, std::fstream::out | std::fstream::app);
}

vector<Track> CSVComponent::readCSV()
{
    vector<Track> tempTrack;
    
    std::ifstream myFile;
    myFile.open(csvPath, std::ifstream::in);
    
    if (myFile.is_open())
    {
        string line;
        std::cout << "CSVComponent::readCSV isOpen" << std::endl;
        while (getline(myFile, line))
        {
            try
            {
                std::vector<std::string> tempString = tokenise(line, ',');
                tempTrack.push_back( Track(tempString[0], tempString[2], tempString[1]));
            }
            catch(const exception& e)
            {
                std::cout << "CSVComponent::readCSV bad data!" << std::endl;
            }
        }
    }
    else std::cout << "CSVComponent::readCSV Error opening file" << std::endl;
    
    return tempTrack;
}

void CSVComponent::writeCSV(std::vector<Track>* trackTitles)
{
    std::ofstream myFile;
    myFile.open(csvPath, std::ofstream::out);

    if (myFile.is_open()) std::cout << "CSVComponent::writeCSV isOpen" << std::endl;
    else std::cout << "CSVComponent::writeCSV Error opening file" << std::endl;
    
    for (Track track : *trackTitles)
    {
        myFile << track.songName << "," << track.audioURL << "," << track.songLength << "\n";
    }
    
    myFile.close();
}

std::vector<string> CSVComponent::tokenise(string csvLine, char separator)
{
    std::vector <string> tokens;
    signed int start, end; // delineate the position of the tokens
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end-start);
        else token = csvLine.substr(start, csvLine.length()-start);
        tokens.push_back(token);
        start = end + 1;
    }
    while (end > 0);
    
    return tokens;
};
