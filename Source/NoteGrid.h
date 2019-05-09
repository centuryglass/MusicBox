/**
 * @file  NoteGrid.h
 *
 * @brief  Tracks and displays all notes in a music strip grid.
 */

#pragma once
#include "JuceHeader.h"
#include <map>

class NoteGrid : public Component
{
public:
    NoteGrid();

    virtual ~NoteGrid() { }

    /**
     * @brief  Adds a new note to the grid.
     *
     * @param note  The new note's value.
     *
     * @param beat  The beat where the note occurs.
     */
    void addNote(const int note, const int beat);

    /**
     * @brief  Removes a note from the grid, if found.
     *
     * @param note  The note value to remove.
     *
     * @param beat  The beat containing the note to remove.
     */
    void removeNote(const int note, const int beat);

    /**
     * @brief  Adds a note to the grid if it isn't already there, removes it if
     *         it is.
     *
     * @param note  The note value to toggle.
     *
     * @param beat  The beat containing the note to toggle.
     */
    void toggleNote(const int note, const int beat);

    /**
     * @brief  Gets all notes at a specific beat.
     *
     * @param beat  The beat to access.
     *
     * @return      All notes at the given beat.
     */
    Array<int> getNotes(const int beat);

    /**
     * @brief  Sets the scale used to convert beat and note indices to 
     *         points within the component.
     *
     * @param noteWidth   The space in pixels between note values.
     *
     * @param beatHeight  The space in pixels between beats.
     */
    void setScale(const float noteWidth, const float beatHeight);

private:
    /**
     * @brief  Draws all notes in the grid.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(Graphics& g) override;

    // Space between notes in pixels:
    float noteWidth = 0;
    // Space between beats in pixels:
    float beatHeight = 0;
    // Maps each beat to the list of all notes in that beat.
    std::map<int, Array<int>> noteMap;
};
