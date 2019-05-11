#include "MusicFile.h"
#include "NoteGrid.h"
#include "Windows_Info.h"
#include "Assets.h"

namespace Svg
{
    static const constexpr float yStart          = 74.956;
    static const constexpr float yTop            = 14.944;
    static const constexpr float xColumn1        = 59.706;
    static const constexpr float xColumn2        = 214.723;
    static const constexpr float offset          = 7.564;
    static const constexpr int columnHeight      = 72;
    static const constexpr int startColumnHeight = 63;

    static const constexpr char* firstPage = "musicStrip_svg";
    static const constexpr char* nextPage  = "musicStrip2_svg";
    static const constexpr int endTagLength = 14;

    static const String noteText = String("<rect\n")
            + "style=\"opacity:0.6;vector-effect:none;fill:#f9f900;"
            + "fill-opacity:0.4;fill-rule:evenodd;stroke:#000000;"
            + "stroke-width:1;stroke-linecap:butt;"
            + "stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;"
            + "stroke-dashoffset:1079.55761719;stroke-opacity:1\"\n"
            + "width=\"4\"\n"
            + "height=\"3\"\n";

    // To complete the note, just add:
    //   id="noteID"
    //   x="xPos"
    //   y="yPos" />
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
        if (! selectedFile.existsAsFile() || selectedFile.getSize() == 0)
        {
            String path = selectedFile.getFullPathName();
            int extensionIndex = path.lastIndexOfChar('.');
            if (extensionIndex > 0)
            {
                path = path.substring(0, extensionIndex);
            }
            path = path + ".mb";
            DBG("Path=" << path);
            selectedFile = File(path);
        }
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
    writeToSVG();
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

static int columnIndex(const int beatNum)
{
    if (beatNum < Svg::startColumnHeight)
    {
        return 0;
    }
    return 1 + (beatNum - Svg::startColumnHeight) / Svg::columnHeight;
}

static int indexInColumn(const int beatNum)
{
    if (beatNum < Svg::startColumnHeight)
    {
        return beatNum;
    }
    return (beatNum - Svg::startColumnHeight) % Svg::columnHeight;
}


static int fileIndex(const int beatNum)
{
    return columnIndex(beatNum) / 2;
}


// Writes all data to SVG note files.
void MusicFile::writeToSVG()
{
    // Load first file:
    StringArray fileTexts;
    fileTexts.add(Assets::findAssetFile(Svg::firstPage).loadFileAsString());

    // Add all beats in the beatMap:
    for (const auto& iter : noteMap)
    {
        // Calculate beat row position:
        const int& beat = iter.first;
        const int column = columnIndex(beat);
        const int pageColumn = column % 2;
        const int columnBeat = indexInColumn(beat);
        const int beatFileIdx = fileIndex(beat);
        const int yPos = (column == 0 ? Svg::yStart : Svg::yTop) 
                + columnBeat * Svg::offset;

        // Add new pages if necessary:
        while (beatFileIdx >= fileTexts.size())
        {
            fileTexts.add(Assets::findAssetFile(Svg::nextPage)
                    .loadFileAsString());
        }

        // Get the appropriate file page string reference:
        String& fileString = fileTexts.getReference(beatFileIdx);

        // Find and add all notes at the beat:
        for (const int& note : iter.second)
        {
            const int xPos = (pageColumn == 0 ? Svg::xColumn1 : Svg::xColumn2) 
                    + note * Svg::offset;
            const String noteText(Svg::noteText + "x=\"" + String(xPos) 
                    + "\"\ny=\"" + String(yPos) + "\" />\n");
            const int endIndex = fileString.length() - Svg::endTagLength;
            fileString = fileString.substring(0, endIndex) + noteText 
                    + fileString.substring(endIndex);
        }
    }

    // Get rid of .mb extension to replace with .svg:
    String filename = path.dropLastCharacters(3);
    // Save file text as .svg:
    for (int i = 0; i < fileTexts.size(); i++)
    {
        File svgFile(filename + String(i + 1) + ".svg");
        svgFile.replaceWithText(fileTexts.getReference(i));
    }
}
