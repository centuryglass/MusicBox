/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 8 May 2019 2:58:15pm
    Author:  anthony

  ==============================================================================
*/

#include "LookAndFeel.h"
#include "Widgets_DrawableImage.h"

MusicBox::LookAndFeel::LookAndFeel()
{
    setColour(Widgets::DrawableImage::imageColour0Id, Colour(0xffffffff));
    setColour(Widgets::DrawableImage::imageColour1Id, Colour(0xff000000));
    setColour(Widgets::DrawableImage::imageColour2Id, Colour(0xffff0000));
    setColour(Widgets::DrawableImage::imageColour3Id, Colour(0xff00ff00));
    setColour(Widgets::DrawableImage::imageColour4Id, Colour(0xff0000ff));
    setColour(ResizableWindow::backgroundColourId, Colour(0xffffffff));
}