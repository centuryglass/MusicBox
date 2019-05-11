#include "NoteGrid.h"

// Default colour value to use when drawing notes.
static const Colour defaultColour(0xff000000);
// Colour value to use for notes in highlighted beats.
static const Colour highlightColour(0xffdcf70d);

NoteGrid::NoteGrid()
{
    setInterceptsMouseClicks(false, false);
}


// Adds a new note to the grid.
void NoteGrid::addNote(const int note, const int beat)
{
    noteMap[beat].add(note);
    //DBG("Added note "  << note << ", there are now "
    //        << noteMap[beat].size() << " note(s) at beat "
    //        << beat);
    resetHighlighting();
    repaint();
}


// Removes a note from the grid, if found.
void NoteGrid::removeNote(const int note, const int beat)
{
    noteMap[beat].removeAllInstancesOf(note);
    repaint();
}


// Adds a note to the grid if it isn't already there, removes it if it is.
bool NoteGrid::toggleNote(const int note, const int beat)
{
    if (noteMap[beat].contains(note))
    {
        removeNote(note, beat);
        return false;
    }
    addNote(note, beat);
    return true;
}


// Gets all notes at a specific beat.
Array<int> NoteGrid::getNotes(const int beat, const bool highlight)
{
    if (highlight)
    {
        const MessageManagerLock mmLock;
        beatColors[beat] = highlightColour;
    }
    return noteMap[beat];
}


// Removes all playback highlighting.
void NoteGrid::resetHighlighting()
{
    beatColors.clear();
}

// Sets the scale used to convert beat and note indices to points within the
// component.
void NoteGrid::setScale(const float noteWidth, const float beatHeight)
{
    this->noteWidth = noteWidth;
    this->beatHeight = beatHeight;
    repaint();
}


// Gets the length of the entire collection of stored notes in beats.
int NoteGrid::getBeatCount() const
{
    int maxBeat = 0;
    for (const auto& iter : noteMap)
    {
        if (! iter.second.isEmpty() && iter.first >= maxBeat)
        {
            maxBeat = iter.first + 1;
        }
    }
    return maxBeat;
}


// Gets all notes stored in the note grid.
std::map<int, Array<int>> NoteGrid::getNoteMap() const
{
    return noteMap;
}


// Checks if this NoteGrid contains any data.
bool NoteGrid::isEmpty() const
{
    if (noteMap.empty())
    {
        return true;
    }
    for (const auto& iter : noteMap)
    {
        if(! iter.second.isEmpty())
        {
            return false;
        }
    }
    return true;
}


// Removes all note data from the NoteGrid.
void NoteGrid::clear()
{
    noteMap.clear();
}

// Draws all notes in the grid.
void NoteGrid::paint(Graphics& g)
{
    const float drawnWidth = noteWidth / 2;
    const float drawnHeight = beatHeight / 2;
    const float cornerSize = std::min(drawnWidth / 5, drawnHeight / 5);
    g.setColour(defaultColour);
    for (const auto& iter : noteMap)
    {
        const int& beat = iter.first;
        if (beatColors.count(beat) != 0)
        {
            Colour& beatColor = beatColors[beat];
            if (beatColor == defaultColour)
            {
                beatColors.erase(beat);
            }
            else
            {
                g.setColour(beatColor);
                beatColor = beatColor.darker(0.02);
            }
        }
        const float yPos = (float) iter.first * beatHeight - drawnWidth / 2
                + (float) gridOffset;
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
        g.setColour(defaultColour);
    }
}
