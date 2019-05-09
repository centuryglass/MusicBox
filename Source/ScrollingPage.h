/**
 * @file  ScrollingPage.h
 *
 * @brief  A scrolling container for all music strips in a sequence.
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Widgets_NavButton.h"
#include "Layout_Group_Manager.h"
#include "MusicStrip.h"


class ScrollingPage : public Component, public Button::Listener
{
public:
    /**
     * @brief  Initializes the component layout and creates the first music
     *         strips.
     */
    ScrollingPage();
    
    virtual ~ScrollingPage() { }
    
private:
    /**
     * @brief  Scrolls the list when the navigation buttons are clicked,
     *         creating new music strips when approaching the bottom edge.
     *
     * @param navButton  The up or down navigation button.
     */
    void buttonClicked(Button* navButton) override;
    
    /**
     * @brief  Applies the page layout when the component's bounds change.
     */
    void resized() override;
    
    /**
     * @brief  Gets the bounds relative to the component where a MusicStrip
     *         should be drawn.
     *
     * @param index  The index of a strip within the sequence of MusicStrip
     *               objects.
     *
     * @return       The bounds where a strip with that index would be located
     *               given the current list position.
     */
    Rectangle<int> getStripBounds(const int index) const;
    
    // Manages the layout of the navigation buttons:
    Layout::Group::Manager layoutManager;
    // Used to scroll up in the page:
    Widgets::NavButton upButton;
    // Used to scroll down in the page:
    Widgets::NavButton downButton;
    // The current scrolling offset, measured in number of music strips. 
    float stripNum = 0;
    // The sequence of music strip objects.
    OwnedArray<MusicStrip> musicStrips;
};
