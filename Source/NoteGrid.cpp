#include "NoteGrid.h"


NoteGrid::NoteGrid()
{
    setInterceptsMouseClicks(false, false);
}


// Adds a new note to the grid.
void NoteGrid::addNote(const int note, const int beat)
{
    noteMap[beat].add(note);
    DBG("Added note "  << note << ", there are now "
            << noteMap[beat].size() << " note(s) at beat "
            << beat);
    repaint();
}


// Removes a note from the grid, if found.
void NoteGrid::removeNote(const int note, const int beat)
{
    noteMap[beat].removeAllInstancesOf(note);
    repaint();
}


// Adds a note to the grid if it isn't already there, removes it if it is.
void NoteGrid::toggleNote(const int note, const int beat)
{
    if (noteMap[beat].contains(note))
    {
        removeNote(note, beat);
    }
    else
    {
        addNote(note, beat);
    }
}


// Gets all notes at a specific beat.
Array<int> NoteGrid::getNotes(const int beat)
{
    return noteMap[beat];
}


// Sets the scale used to convert beat and note indices to points within the
// component.
void NoteGrid::setScale(const float noteWidth, const float beatHeight)
{
    this->noteWidth = noteWidth;
    this->beatHeight = beatHeight;
    repaint();
}


// Draws all notes in the grid.
void NoteGrid::paint(Graphics& g)
{
    const float drawnWidth = noteWidth / 2;
    const float drawnHeight = beatHeight / 3;
    const float cornerSize = std::min(drawnWidth / 5, drawnHeight / 5);
    g.setColour(Colour(0xff000000));
    for (const auto& iter : noteMap)
    {
        const float yPos = (float) iter.first * beatHeight - drawnWidth / 2;
        for (const int& note : iter.second)
        {
            const int xPos = (float) note * noteWidth - drawnHeight / 2;
            if(yPos < 0)
            {
                DBG(xPos << ", " << yPos);
            }
            g.fillRoundedRectangle(xPos, yPos, drawnWidth, drawnHeight,
                    cornerSize);
        }

    }
}
