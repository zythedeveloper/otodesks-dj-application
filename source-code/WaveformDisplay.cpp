/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 13 Feb 2023 4:37:51pm
    Author:  ZenYu

  ==============================================================================
*/

#include "WaveformDisplay.h"

/*----- Constructor -----*/
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse)
: audioThumb(1000 ,formatManagerToUse, cacheToUse),
  sourceLoaded(false),
  position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}



/*----- Public Function -----*/
void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    
    if (sourceLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 1, getHeight());
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear(); 
    sourceLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (!sourceLoaded) cout << "WaveformDisplay::loadURL unsuccessful" << endl;
}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if(pos != position)
    {
        position = pos;
        repaint();
    }
}
