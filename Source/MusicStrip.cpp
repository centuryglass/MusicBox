#include "MusicStrip.h"

static const constexpr char* startPath = "musicStart_svg";
static const constexpr char* midPath   = "musicMid_svg";
static const constexpr int width = 107;
static const constexpr int height = 243;
static const constexpr int paddedWidth = 165;
static const constexpr float yOffset = 0.05;


// Creates the component, loading its image.
MusicStrip::MusicStrip(const bool isStart) :
isStart(isStart),
backgroundImage((isStart ? startPath : midPath),
        RectanglePlacement::stretchToFit)
{
    addAndMakeVisible(backgroundImage);
}


// Gets the x-position of a note within the component.
int MusicStrip::getNoteX(const int note) const
{
    if (note < 0 || note > 14)
    {
        DBG("Invalid note " << note);
        return -1;
    }
    return imageBounds.getX() + getNoteWidth() * note;
}


// Gets the y-position of a beat within the component.
int MusicStrip::getBeatY(const int beat) const
{
    if (beat < 0 || beat > getBeatCount())
    {
        DBG("Invalid beat " << beat);
        return -1;
    }
    return getBeatTop() + getBeatHeight() * beat;
}


// Gets the closest note to an x-coordinate if within a certain error range.
int MusicStrip::getClosestNote(const int xPos, const int maxError) const
{
    const float xOffset = xPos - imageBounds.getX();
    if (xOffset < -maxError || xOffset > (imageBounds.getWidth() + maxError))
    {
        return -1;
    }
    const float noteWidth = getNoteWidth();
    int closestNote = xOffset / noteWidth;
    float error = xOffset - (noteWidth * (float) closestNote);
    if (error > (noteWidth / 2))
    {
        closestNote++;
        error = noteWidth - error;
    }
    if (error > maxError)
    {
        return -1;
    }
    return closestNote;
}


// Gets the closest beat to a y-coordinate if within a certain error range.
int MusicStrip::getClosestBeat(const int yPos, const int maxError) const
{
    const float yTop = getBeatTop();
    const float stripHeight = (float) imageBounds.getHeight() 
            - (yTop - (float) imageBounds.getY());
    const float yOffset = (float) yPos - yTop;
    if (yOffset < -maxError || yOffset > (stripHeight + maxError))
    {
        return -1;
    }
    const float beatHeight = getBeatHeight();
    int closestBeat = std::floor(yOffset / beatHeight);
    float error = yOffset - (beatHeight * (float) closestBeat);
    if (error > (beatHeight / 2))
    {
        closestBeat++;
        error = beatHeight - error;
    }
    if (error > maxError)
    {
        return -1;
    }
    return closestBeat;
}


// Adds a listener to the list of objects receiving noteClicked updates.
void MusicStrip::addListener(Listener* listener)
{
    listeners.addIfNotAlreadyThere(listener);
}


// Removes a listener from the list of objects receiving noteClicked updates.
void MusicStrip::removeListener(Listener* listener)
{
    listeners.removeAllInstancesOf(listener);
}


// Gets the width of a note in the component.
float MusicStrip::getNoteWidth() const
{
    return (float) imageBounds.getWidth() / 14.0;
}


// Gets the height of a beat in the component.
float MusicStrip::getBeatHeight() const
{
    return (float) imageBounds.getHeight() / 32.0;
}
    

// Gets the y-coordinate of the first beat in the component.
float MusicStrip::getBeatTop() const
{
    return imageBounds.getY() + (isStart ? getBeatHeight() * 8.0 : 0.0);
}


// Gets the number of beats within this music strip.
int MusicStrip::getBeatCount() const
{
    return (isStart ? 24 : 32);
}


// Updates the internal image size when the strip's bounds change.
void MusicStrip::resized()
{
    // Recalculate image bounds:
    const float imageRatio = (float) width / (float) height;
    const float stripRatio = (float) getWidth() / (float) getHeight();
    const float imageHeight = getHeight();
    const float imageWidth = imageHeight * imageRatio;
    imageBounds.setBounds(
            (getWidth() - imageWidth) / 2,
            (getHeight() - imageHeight) / 2,
            imageWidth,
            imageHeight);
    const int fullWidth = (float) width * imageBounds.getHeight() 
            / (float) paddedWidth;
    backgroundImage.setBounds(imageBounds.withSizeKeepingCentre(fullWidth,
            imageBounds.getHeight()));
}

// Notifies listeners when notes are clicked.
void MusicStrip::mouseDown(const MouseEvent& event)
{
    const int maxError = imageBounds.getWidth() / 40;
    const int note = getClosestNote(event.position.x, maxError);
    const int beat = getClosestBeat(event.position.y, maxError);
    if (note != -1 && beat != -1)
    {
        //DBG("note " << note << ", beat " << beat << " clicked, notifying "
        //        << listeners.size() << " listeners.");
        for (Listener* listener : listeners)
        {
            listener->noteClicked(this, note, beat);
        }
    }
}


// Optionally draws calculated coordinates for debugging purposes.
void MusicStrip::paint(Graphics& g) 
{   
    /*
    const int noteCount = 15;

    // draw image bounds for testing:
    g.setColour(Colour(0x55ff0000));
    g.fillRect(imageBounds);

    // draw note lines for testing:
    g.setColour(Colour(0xffff0000));
    for (int x = 0; x < noteCount; x++)
    {
        g.fillRect(getNoteX(x), 0, 1, getHeight());
    }

    // draw beat lines for testing:
    g.setColour(Colour(0xff00ff00));
    for (int y = 0; y < getBeatCount(); y++)
    {
        g.fillRect(0, getBeatY(y), getWidth(), 1);
    }

    // Test beat checking:
    const int maxError = imageBounds.getWidth() / 40;
    for (int x = 0; x < getWidth(); x++)
    {
        for (int y = 0; y < getHeight(); y++)
        {
            int note = getClosestNote(x, maxError);
            int beat = getClosestBeat(y, maxError);
            if ((note != -1) && (beat != -1))
            {
                g.setColour(Colour(
                            (uint8) 0xff / noteCount * note,
                            (uint8) 0,
                            (uint8) 0xff / getBeatCount() * beat,
                            (uint8) 0xff));
                g.fillRect(x, y, 1, 1);
            }
        }
    }
    */
}
