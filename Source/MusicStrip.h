/*
  ==============================================================================

    MusicStrip.h
    Created: 8 May 2019 3:46:06pm
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Widgets_DrawableImage.h"

class MusicStrip : public juce::Component
{
public:
    MusicStrip(const bool isStart);
    
    virtual ~MusicStrip() { }

    int getNoteX(const int note) const;

    int getBeatY(const int beat) const;

    int getClosestNote(const int xPos, const int maxError) const;

    int getClosestBeat(const int yPos, const int maxError) const;
    
private:
    void resized() override;

    void mouseDown(const MouseEvent& event);
    
    void paint(Graphics& g) override;

    float getNoteWidth() const;

    float getBeatHeight() const;

    float getBeatTop() const;
    
    const bool isStart;
    Widgets::DrawableImage backgroundImage;
    Rectangle<int> imageBounds;
};
