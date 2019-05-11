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
#include "NoteGrid.h"
#include "NotePlayer.h"
#include "PlaybackTimer.h"


class ScrollingPage : public Component, public Button::Listener,
    public MusicStrip::Listener
{
public:
    /**
     * @brief  Initializes the component layout and creates the first music
     *         strips.
     *
     * @param notePlayer  Handles audio playback for the ScrollingPage.
     */
    ScrollingPage(NotePlayer& notePlayer);
    
    virtual ~ScrollingPage() { }
    
private:
    /**
     * @brief  Passes all page button actions on to the appropriate functions.
     *
     * @param button  One of the buttons on the page.
     */
    void buttonClicked(Button* button) override;
    
    /**
     * @brief  Toggles a note in the NoteGrid when it is clicked in a
     *         MusicStrip.
     *
     * @param strip  The MusicStrip where the click occurred.
     *
     * @param note   The clicked note's value.
     *
     * @param beat   The beat within the MusicStrip where the note is
     *               located.
     */
    void noteClicked(MusicStrip* strip, const int note, const int beat) 
            override;

    /**
     * @brief  Plays back all notes in the music strip sequence.
     */
    void startPlayback();

    /**
     * @brief  Applies the page layout when the component's bounds change.
     */
    void resized() override;

    /**
     * @brief  Draws the playback bar over the music strip.
     *
     * @param g  The juce graphics context.
     */
    void paint(Graphics& g) override;
    
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

    /**
     * @brief  Scrolls the page forwards or backwards, animating the component
     *         transition.
     *
     * @param offset  The distance to move all strips, as a fraction of the
     *                height of one strip.
     */
    void changeStripPosition(const float offset);

    /**
     * @brief  Scrolls the music strip sequence to the appropriate location for
     *         its current index.
     */
    void scrollToCurrentPos();

    /**
     * @brief  Clears all song data after getting confirmation from the user.
     */
    void clearData();

    /**
     * @brief  Saves all song data to a file selected by the user.
     */
    void saveData();

    /**
     * @brief  Loads all song data from a file selected by the user.
     */
    void loadData();
    
    // Manages the layout of the navigation buttons:
    Layout::Group::Manager layoutManager;
    // Used to scroll up in the page:
    Widgets::NavButton upButton;
    // Used to scroll down in the page:
    Widgets::NavButton downButton;

    // Sets playback speed:
    Label bpmLabel;
    TextEditor bpmEditor;

    // Starts playback:
    Label playLabel;
    Widgets::NavButton playButton;

    // Save, load, or discard song data.
    TextButton clearButton;
    TextButton saveButton;
    TextButton loadButton;

    // The current scrolling offset, measured in number of music strips. 
    float stripNum = 0;
    // The sequence of music strip objects.
    OwnedArray<MusicStrip> musicStrips;
    // Shows all music notes on the strip sequence.
    NoteGrid noteGrid;
    // Plays notes using audio samples.
    NotePlayer& notePlayer;
    // Plays back all notes in the sequence.
    PlaybackTimer playbackTimer;
};
