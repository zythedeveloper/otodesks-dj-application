/*
  ==============================================================================

    LibraryGUI.h
    Created: 2 Mar 2023 7:43:39pm
    Author:  ZenYu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <cmath>
#include "Track.h"
#include "DeskGUI.h"
#include "DJAudioPlayer.h"
#include "LibraryComponent.h"

using namespace std;

class LibraryGUI   :    public juce::Component,
                        public juce::TableListBoxModel,
                        public juce::FileDragAndDropTarget,
                        public juce::Button::Listener
{
public:
    LibraryGUI(std::vector<Track>* trackTitles, DeskGUI* deskGUI1, DeskGUI* deskGUI2);
    ~LibraryGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /* Button::Listener Pure Virtual Functions */
    void buttonClicked(juce::Button* button) override;
    
    /* TableListBoxModel Pure Virtual Functions */
    int getNumRows() override;
    void paintRowBackground (juce::Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /* juce::FileDragAndDropTarget Pure Virtual Function */
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;

private:
    std::string resourcePath;
    std::vector<Track>* trackTitles;
    juce::TableListBox tableComponent;
    juce::TextEditor searchBar{"searchBar", 0};
    
    juce::ImageButton searchButton{"searchButton"};
    juce::ImageButton clearButton{"clearButton"};
    
    DeskGUI* deskGUI1;
    DeskGUI* deskGUI2;
    
    DJAudioPlayer virtualPlayer{trackTitles};
    
    LibraryComponent libraryComponent{&virtualPlayer, trackTitles};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryGUI)
};
