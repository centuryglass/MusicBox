/**
 * @file  MusicFile.h
 * 
 * @brief  Reads and writes music box strip data in files.
 */

#pragma once
#include "JuceHeader.h"
#include <map>

class NoteGrid;

class MusicFile
{
public:
    /**
     * @brief  Uses a file picker window to select or create a music file
     *         for reading or writing.
     *
     * @param loading  True if loading a track, false if saving a track.
     *
     * @return         The created music file.
     */
    static MusicFile openFile(const bool loading);

    /**
     * @brief  Creates or loads a music file.
     *
     * @param path  The path to an existing music file, or the path where a new
     *              music file should be written.
     */
    MusicFile(const String path);

    virtual ~MusicFile() { }

    /**
     * @brief  Checks if this MusicFile has a valid path.
     *
     * @return  Whether the path is the location of a file, or a location where
     *          a file may be created.
     */
    bool validFile() const;

    /**
     * @brief  Gets the BPM value stored in a music file.
     *
     * @return  Beats per minute to use when playing the stored song.
     */
    int getBPM() const;

    /**
     * @brief  Sets the BPM value to store in a music file.
     *
     * @param bpm  Beats per minute to use when playing the stored song.
     */
    void setBPM(const int bpm);

    /**
     * @brief  Imports note data from a NoteGrid object.
     *
     * @param noteGrid  A NoteGrid storing song data.
     */
    void importNoteGrid(NoteGrid* noteGrid);

    /**
     * @brief  Exports file data to a NoteGrid object.
     *
     * @param noteGrid  A NoteGrid that will store the file's song data.
     */
    void exportToNoteGrid(NoteGrid* noteGrid);

    /**
     * @brief  Writes all data to the MusicFile's path.
     *
     * @return  Whether the write was successful.
     */
    bool writeToFile();

private:
    String path;
    int bpm = 60;
    std::map<int, Array<int>> noteMap;
};
