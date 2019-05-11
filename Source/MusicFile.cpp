#include "MusicFile.h"
#include "NoteGrid.h"
#include "Windows_Info.h"

namespace SvgVals
{
    static const constexpr float yStart = 75.956;
    static const constexpr float yTop = 15.444;
    static const constexpr float xColumn1 = 61.206;
    static const constexpr float xColumn2 = 216.223;
    static const constexpr float offset = 7.564;
}

// Uses a file picker window to select or create a music file for reading or
// writing.
MusicFile MusicFile::openFile(const bool loading)
{
    static WildcardFileFilter filter("*.mb", "", "MusicBox files");
    const int modeFlags = (loading ?  FileBrowserComponent::openMode 
            : FileBrowserComponent::saveMode)
            | FileBrowserComponent::canSelectFiles;
    FileBrowserComponent fileBrowser(
            modeFlags,
            File(),
            &filter,
            nullptr);
    FileChooserDialogBox dialogBox("MusicBox Files",
            (loading ? "Select a music box file to load."
                    : "Select a music box file to save."),
            fileBrowser,
            false,
            Colour(0xffffffff));
    Rectangle<int> size = Windows::Info::getBounds();
    if (dialogBox.show(size.getWidth(), size.getHeight()))
    {
        File selectedFile = fileBrowser.getSelectedFile(0);
        return MusicFile(selectedFile.getFullPathName());
    }
    return MusicFile("");
}


// Creates or loads a music file.
MusicFile::MusicFile(const String path) : path(path)
{
    File musicFile(path);
    if (musicFile.existsAsFile())
    {
        StringArray lines;
        musicFile.readLines(lines);
        for (const String& line : lines)
        {
            if (line.contains(":"))
            {
                int idx = line.indexOf(":");
                int beat = line.substring(0, idx).getIntValue();
                Array<int> notes;
                StringArray noteStrings 
                        = StringArray::fromTokens(line.substring(idx), false);
                for (const String& note : noteStrings)
                {
                    if (note.containsOnly("0123456789"))
                    {
                        notes.add(note.getIntValue());
                    }
                }
                if (! notes.isEmpty())
                {
                    noteMap[beat] = notes;
                }
            }
            else
            {
                int lineNum = line.getIntValue();
                if (lineNum > 0)
                {
                    bpm = lineNum;
                }
            }
        }
    }
    else if (! musicFile.hasWriteAccess())
    {
        this->path = "";
    }
}

// Checks if this MusicFile has a valid path.
bool MusicFile::validFile() const
{
    return path.isNotEmpty() && File(path).hasWriteAccess();
}

// Gets the BPM value stored in a music file.
int MusicFile::getBPM() const
{
    return bpm;
}


// Sets the BPM value to store in a music file.
void MusicFile::setBPM(const int bpm)
{
    this->bpm = bpm;
}


// Imports note data from a NoteGrid object.
void MusicFile::importNoteGrid(NoteGrid* noteGrid)
{
    noteMap = noteGrid->getNoteMap();
}


// Exports file data to a NoteGrid object.
void MusicFile::exportToNoteGrid(NoteGrid* noteGrid)
{
    for (const auto& iter : noteMap)
    {
        if (! iter.second.isEmpty())
        {
            const int beat = iter.first;
            for (const int& note : iter.second)
            {
                noteGrid->addNote(note, beat);
            }
        }
    }
}


// Writes all data to the MusicFile's path.
bool MusicFile::writeToFile()
{
    String fileOutput(bpm);
    for (const auto& iter : noteMap)
    {
        if (iter.second.isEmpty())
        {
            continue;
        }
        String line(iter.first);
        line += ":";
        for (const int& note : iter.second)
        {
            if(line.length() > 1)
            {
                line += " ";
            }
            line += String(note);
        }
        fileOutput += "\n";
        fileOutput += line;
    }
    File outFile(path);
    return outFile.replaceWithText(fileOutput);
}
