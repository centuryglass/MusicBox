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
     *
     * @return      True if and only if a note was added.
     */
    bool toggleNote(const int note, const int beat);

    /**
     * @brief  Gets all notes at a specific beat.
     *
     * @param beat       The beat to access.
     *
     * @param highlight  If true, notes at the accessed beat will be highlighted
     *                   briefly when drawing the NoteGrid.
     *
     * @return           All notes at the given beat.
     */
    Array<int> getNotes(const int beat, const bool highlight = false);

    /**
     * @brief  Sets the scale used to convert beat and note indices to 
     *         points within the component.
     *
     * @param noteWidth   The space in pixels between note values.
     *
     * @param beatHeight  The space in pixels between beats.
     */
    void setScale(const float noteWidth, const float beatHeight);

    /**
     * @brief  Gets the length of the entire collection of stored notes in
     *         beats.
     *
     * @return  The number of beats it would take to completely play back the
     *          stored notes, plus one.
     */
    int getBeatCount() const;

    /**
     * @brief  Gets all notes stored in the note grid.
     *
     * @return  A copy of the grid's note map, mapping each beat number to the
     *          list of notes played on that beat.
     */
    std::map<int, Array<int>> getNoteMap() const;

    /**
     * @brief  Checks if this NoteGrid contains any data.
     *
     * @return  Whether the NoteGrid currently contains no notes.
     */
    bool isEmpty() const;

    /**
     * @brief  Removes all note data from the NoteGrid.
     */
    void clear();

    // Number of pixels between the upper bounds and the first note.
    static const constexpr int gridOffset = 15;
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

    std::map<int, Colour> beatColors;
};
