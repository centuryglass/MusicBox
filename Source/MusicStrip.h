
/**
 * @file  MusicStrip.h
 *
 * @brief  Draws a section of the music box paper, and keeps track of the
 *         locations where it may have notes.
 */

#pragma once
#include "JuceHeader.h"
#include "Widgets_DrawableImage.h"

class MusicStrip : public juce::Component
{
public:
    /**
     * @brief  Creates the component, loading its image.
     *
     * @param isStart  Whether this strip is the start of the MusicStrip
     *                 column.
     */
    MusicStrip(const bool isStart);
    
    virtual ~MusicStrip() { }

    /**
     * @brief  Gets the x-position of a note within the component.
     *
     * @param note  A note value between 0 and 14, inclusive.
     *
     * @return      The note's x-coordinate within the component bounds, or -1
     *              if the note parameter is invalid.
     */
    int getNoteX(const int note) const;

    /**
     * @brief  Gets the y-position of a beat within the component.
     *
     * @param beat  A beat number that is at least zero and is less than the
     *              number of beats in the component.
     *
     * @return      The beat's y-coordinate within the component bounds, or -1
     *              if the beat parameter is invalid.
     */
    int getBeatY(const int beat) const;

    /**
     * @brief  Gets the closest note to an x-coordinate if within a certain
     *         error range.
     *
     * @param xPos      An x-coordinate within the component.
     *
     * @param maxError  The maximum distance the x-coordinate may be from the
     *                  note and still return its value.
     *
     * @return          The closest note index, or -1 if no note is within the
     *                  error range.
     */
    int getClosestNote(const int xPos, const int maxError) const;

    /**
     * @brief  Gets the closest beat to a y-coordinate if within a certain
     *         error range.
     *
     * @param yPos      A y-coordinate within the component.
     *
     * @param maxError  The maximum distance the y-coordinate may be from the
     *                  beat and still return its value.
     *
     * @return          The closest beat index, or -1 if no beat is within the
     *                  error range.
     */
    int getClosestBeat(const int yPos, const int maxError) const;

    /**
     * @brief  An abstract basis for classes that track when notes are clicked
     *         within the MusicStrip.
     */
    class Listener
    {
    public:
        // Only MusicStrip may call noteClicked()
        friend MusicStrip;

        Listener() { }

        virtual ~Listener() { }

    private:
        /**
         * @brief  Notifies the listener that a note was clicked.
         *
         * @param strip  The MusicStrip where the click occurred.
         *
         * @param note   The clicked note's value.
         *
         * @param beat   The beat within the MusicStrip where the note is
         *               located.
         */
        virtual void noteClicked(MusicStrip* strip, const int note,
                const int beat) = 0;
    };

    /**
     * @brief  Adds a listener to the list of objects receiving noteClicked
     *         updates.
     *
     * @param listener  The object requesting noteClicked updates.
     */
    void addListener(Listener* listener);

    /**
     * @brief  Removes a listener from the list of objects receiving noteClicked
     *         updates.
     *
     * @param listener  The object that should no longer receive updates.
     */
    void removeListener(Listener* listener);
    
private:
    /**
     * @brief  Updates the internal image size when the strip's bounds change.
     */
    void resized() override;

    /**
     * @brief  Notifies listeners when notes are clicked.
     *
     * @param event  The mouse click event.
     */
    void mouseDown(const MouseEvent& event) override;
    
    /**
     * @brief  Optionally draws calculated coordinates for debugging purposes.
     *
     * @param g  The JUCE graphics context drawing the component.
     */
    void paint(Graphics& g) override;

    /**
     * @brief  Gets the width of a note in the component.
     *
     * @return  The number of pixels between note values.
     */
    float getNoteWidth() const;

    /**
     * @brief  Gets the height of a beat in the component.
     *
     * @return   The number of pixels between beat values.
     */
    float getBeatHeight() const;

    /**
     * @brief  Gets the y-coordinate of the first beat in the component.
     *
     * @return  The first beat's distance from the top of the component.
     */
    float getBeatTop() const;
    
    // Whether this is the first strip in a sequence:
    const bool isStart;
    // Draws the music strip image:
    Widgets::DrawableImage backgroundImage;
    // Keeps track of the music staff bounds within the component:
    Rectangle<int> imageBounds;
    // All registered listener objects:
    Array<Listener*> listeners;
};
