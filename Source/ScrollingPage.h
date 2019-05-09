/*
  ==============================================================================

    ScrollingPage.h
    Created: 8 May 2019 2:31:43am
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Widgets_NavButton.h"
#include "Layout_Group_Manager.h"
#include "MusicStrip.h"


class ScrollingPage : public Component, public Button::Listener
{
public:
    ScrollingPage();
    
    virtual ~ScrollingPage() { }
    
private:
    void buttonClicked(Button* navButton) override;
    
    void resized() override;
    
    Rectangle<int> getStripBounds(const int index) const;
    
    Layout::Group::Manager layoutManager;
    Widgets::NavButton upButton;
    Widgets::NavButton downButton;
    
    float stripNum = 0;
    OwnedArray<MusicStrip> musicStrips;
};
