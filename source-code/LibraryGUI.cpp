/*
  ==============================================================================

    LibraryGUI.cpp
    Created: 2 Mar 2023 7:43:39pm
    Author:  ZenYu

  ==============================================================================
*/

#include "LibraryGUI.h"

LibraryGUI::LibraryGUI(std::vector<Track>* _trackTitles, DeskGUI* _deskGUI1, DeskGUI* _deskGUI2)
: trackTitles(_trackTitles), deskGUI1(_deskGUI1), deskGUI2(_deskGUI2)
{
    // getting the parent path to Otodecks Application
    juce::String absJucePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getFullPathName();
    // concatenate the parent path with our csv path
    resourcePath = absJucePath.toStdString() + "/OtoDesksResource/";
    
    juce::Image imgSearch = ImageFileFormat::loadFrom(juce::File{ resourcePath + "search.png" });
    juce::Image imgClear = ImageFileFormat::loadFrom(juce::File{ resourcePath + "reset.png" });
    
    // header of table
    tableComponent.getHeader().addColumn("Track Title", 1, 500);
    tableComponent.getHeader().addColumn("Duration", 2, 75);
    tableComponent.getHeader().addColumn("player1", 3, 100);
    tableComponent.getHeader().addColumn("player2", 4, 100);
    
    tableComponent.setModel(this);

    // make components visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);
    addAndMakeVisible(searchButton);
    addAndMakeVisible(clearButton);
    
    // add event listener
    searchButton.addListener(this);
    clearButton.addListener(this);

    // set the image
    searchButton.setImages(false, true, true, imgSearch,
                         1.0f, juce::Colour{}, imgSearch,
                         1.0f, juce::Colour{}, imgSearch,
                         1.0f, juce::Colour{}, 0.0f);
    clearButton.setImages(false, true, true, imgClear,
                         1.0f, juce::Colour{}, imgClear,
                         1.0f, juce::Colour{}, imgClear,
                         1.0f, juce::Colour{}, 0.0f);
    
    // extract the previous music library from csv
    *trackTitles = libraryComponent.loadCSV();
}

LibraryGUI::~LibraryGUI()
{
}

void LibraryGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll (juce::Colours::grey);
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LibraryGUI::resized()
{
    int rowHeight = getHeight() / 10;
    tableComponent.setBounds(0, 0, getWidth(), rowHeight * 9);
    searchBar.setBounds(0, rowHeight * 9, getWidth() * 0.3, rowHeight);
    searchButton.setBounds(getWidth() * 0.3 , rowHeight * 9, 50, rowHeight);
    clearButton.setBounds(getWidth() * 0.3 + 50, rowHeight * 9, 50, rowHeight);
}

void LibraryGUI::buttonClicked(juce::Button* button)
{
    // button from table has specific ID
    try
    {
        int id = std::stoi(button->getComponentID().toStdString());
        
        if (id > -1)
        {
            if (id < getNumRows()) deskGUI1->loadURL(juce::URL{ juce::File{ (*trackTitles)[id % getNumRows()].audioURL } });
            else if (id >= getNumRows()) deskGUI2->loadURL(juce::URL{ juce::File{ (*trackTitles)[id % getNumRows()].audioURL } });
        }
    } catch (...) {}
    
    if (button == &searchButton)
    {
        std::string input = searchBar.getText().toStdString();
        for (unsigned int i=0; i<trackTitles->size(); i++)
        {
            if (input == (*trackTitles)[i].songName) tableComponent.selectRow(i, false, true);
        }
    }
    else if (button == &clearButton) searchBar.clear();
}

int LibraryGUI::getNumRows()
{
    return (int)trackTitles->size();
}

void LibraryGUI::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) g.fillAll(Colours::orange);
    else g.fillAll(Colours::white);
}

void LibraryGUI::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.drawRect (getLocalBounds(), 1);
    if (columnId == 1) g.drawText((*trackTitles)[rowNumber].songName, 2, 0, width - 4, height, Justification::centredLeft, true);
    if (columnId == 2) g.drawText((*trackTitles)[rowNumber].songLength, 2, 0, width - 4, height, Justification::centredLeft, true);
}

Component* LibraryGUI::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        juce::TextButton* btn = new juce::TextButton("LOAD");
        juce::String id{ std::to_string(rowNumber) };
        
        btn->setComponentID(id);
        btn->addListener(this);
        btn->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        
        existingComponentToUpdate = btn;
    }
    
    if (columnId == 4)
    {
        juce::TextButton* btn = new juce::TextButton("LOAD");
        juce::String id{ std::to_string(rowNumber + getNumRows()) };

        btn->setComponentID(id);
        btn->addListener(this);
        btn->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        
        existingComponentToUpdate = btn;
    }
    
    return existingComponentToUpdate;
}

bool LibraryGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
    // std::cout << "DeskGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void LibraryGUI::filesDropped (const juce::StringArray& files, int x, int y)
{
    // std::cout << "DeskGUI::filesDropped" << std::endl;
    
    libraryComponent.addTrack(files);
    libraryComponent.saveCSV();
    tableComponent.updateContent();
}
