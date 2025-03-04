/*
  ==============================================================================

    DeskGUI.cpp
    Created: 7 Feb 2023 12:19:21pm
    Author:  ZenYu

  ==============================================================================
*/

#include "DeskGUI.h"

/*----- Constructor -----*/
DeskGUI::DeskGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)
: player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // getting the parent path to Otodecks Application
    juce::String absJucePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getFullPathName();
    // concatenate the parent path with our csv path
    resourcePath = absJucePath.toStdString() + "/OtoDesksResource/";
    
    juce::Image imgVolume = ImageFileFormat::loadFrom(juce::File{ resourcePath + "speaker.png" });
    juce::Image imgSpeed = ImageFileFormat::loadFrom(juce::File{ resourcePath + "speed.png" });
    juce::Image imgNextTrack = ImageFileFormat::loadFrom(juce::File{ resourcePath + "next-track.png" });
    juce::Image imgPreTrack = ImageFileFormat::loadFrom(juce::File{ resourcePath + "previous-track.png" });
    juce::Image imgPlay = ImageFileFormat::loadFrom(juce::File{ resourcePath + "play.png" });
    juce::Image imgPause = ImageFileFormat::loadFrom(juce::File{ resourcePath + "pause.png" });
    
    // make components visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(leftButton);
    addAndMakeVisible(rightButton);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(volComponent);
    addAndMakeVisible(spdComponent);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(nameLabel);
    
    // add event listener
    playButton.addListener(this);
    pauseButton.addListener(this);
    leftButton.addListener(this);
    rightButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // set the range of the slider
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.00000001, 10.0);
    posSlider.setRange(0.0, 1.0);
    
    // change the type of slider
    volSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    
    // hide the indicator of slider
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    // set the image
    playButton.setImages(false, true, true, imgPlay,
                         1.0f, juce::Colour{}, imgPlay,
                         1.0f, juce::Colour{}, imgPlay,
                         1.0f, juce::Colour{}, 0.0f);
    pauseButton.setImages(false, true, true, imgPause,
                         1.0f, juce::Colour{}, imgPause,
                         1.0f, juce::Colour{}, imgPause,
                         1.0f, juce::Colour{}, 0.0f);
    leftButton.setImages(false, true, true, imgPreTrack,
                         1.0f, juce::Colour{}, imgPreTrack,
                         1.0f, juce::Colour{}, imgPreTrack,
                         1.0f, juce::Colour{}, 0.0f);
    rightButton.setImages(false, true, true, imgNextTrack,
                         1.0f, juce::Colour{}, imgNextTrack,
                         1.0f, juce::Colour{}, imgNextTrack,
                         1.0f, juce::Colour{}, 0.0f);
    volComponent.setImage(imgVolume);
    spdComponent.setImage(imgSpeed);
    
    startTimer(50);
}

DeskGUI::~DeskGUI()
{
    stopTimer();
}



/*----- Public Function -----*/
void DeskGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll (juce::Colours::lightslategrey);
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void DeskGUI::resized()
{
    double rowH = getHeight() / 5;
    waveformDisplay.setBounds(0, 0, getWidth()* 5/6, rowH * 3);
    speedSlider.setBounds(getWidth()* 5/6, 0, getWidth()* 1/6, rowH * 5);
    spdComponent.setBounds(getWidth()* 5/6 + 37.5, rowH * 5/2 - 25, 50, 50);
    
    leftButton.setBounds(getWidth()* 2/6 - 60, rowH * 3, 30, rowH);
    playButton.setBounds(getWidth()* 2/6 - 30, rowH * 3, 30, rowH);
    pauseButton.setBounds(getWidth()* 2/6 + 0, rowH * 3, 30, rowH);
    rightButton.setBounds(getWidth()* 2/6 + 30, rowH * 3, 30, rowH);
    
    volComponent.setBounds(getWidth()* 3/6 - 60, rowH * 3, 50 , rowH);
    volSlider.setBounds(getWidth() * 3/6 - 25, rowH * 3, getWidth() * 1/6, rowH);
    posSlider.setBounds(40, rowH * 4, getWidth() * 5/6 -55, rowH/2);
    
    posLabel.attachToComponent(&posSlider, true);
    nameLabel.setBounds(0, rowH * 3, getWidth() * 1/4, rowH);
}

void DeskGUI::loadURL(juce::URL audioFile)
{
    if (audioFile != juce::URL{""})
    {
        std::string songName = audioFile.getLocalFile().getFileNameWithoutExtension().toStdString();
        nameLabel.setText(songName, dontSendNotification);
        player->loadURL(audioFile);
        waveformDisplay.loadURL(audioFile);
    }
    // probably audio source haven't loaded
    else std::cout << "DeskGUI::loadURL path error" << std::endl;
}


void DeskGUI::buttonClicked(juce::Button *button)
{
    if (button == &playButton) player->start();
    else if (button == &pauseButton) player->stop();
    else if (button == &leftButton) loadURL(player->previousTrack());
    else if (button == &rightButton) loadURL(player->nextTrack());
}

void DeskGUI::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &volSlider) player->setGain(slider->getValue());
    if (slider == &speedSlider) player->setSpeed(slider->getValue());
    if (slider == &posSlider) player->setPositionRelative(slider->getValue());
}

void DeskGUI::timerCallback()
{
    // get the current position
    int currentLength = player->getPositionRelative() * player->getSourceLength();
    // convert to minutes:seconds format
    std::string currentString = std::to_string( (int)floor(currentLength / 60) ) + ":" + std::to_string(currentLength % 60);
    
    // update the slider text and value according to the current position
    posLabel.setText(currentString, dontSendNotification);
    if (player->checkSourceLoaded()) posSlider.setValue(player->getPositionRelative());
    
    // update the waveformDisplay according to the current position
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
