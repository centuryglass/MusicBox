#include "ScrollingPage.h"
#include "Layout_Transition_Animator.h"
#include "MusicFile.h"
#include "Windows_Alert.h"
#include <map>

static const constexpr int animationMS = 200;


// Initializes the component layout and creates the first music strips.
ScrollingPage::ScrollingPage(NotePlayer& notePlayer):
upButton(Widgets::NavButton::WindowEdge::up),
downButton(Widgets::NavButton::WindowEdge::down),
playButton(Widgets::NavButton::WindowEdge::right),
notePlayer(notePlayer),
playLabel("Play", "Play:"),
bpmLabel("BPM", "BPM:"),
loadButton("Load Song"),
saveButton("Save Song"),
clearButton("Clear All"),
playbackTimer(notePlayer)
{
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(2,
        { 
            RowItem(20),
            RowItem(&upButton, 2),
            RowItem(4)
        }),
        Row(1,
        { 
            RowItem(14),
            RowItem(&bpmLabel, 2),
            RowItem(&bpmEditor, 1),
            RowItem(1)
        }),
        Row(1,
        { 
            RowItem(14),
            RowItem(&playLabel, 2),
            RowItem(&playButton, 1),
            RowItem(1)
        }),
        Row(15),
        Row(1,
        { 
            RowItem(20),
            RowItem(&saveButton, 2),
            RowItem(4)
        }),
        Row(1,
        { 
            RowItem(20),
            RowItem(&loadButton, 2),
            RowItem(4)
        }),
        Row(1,
        { 
            RowItem(20),
            RowItem(&clearButton, 2),
            RowItem(4)
        }),
        Row(2,
        { 
            RowItem(20),
            RowItem(&downButton, 2),
            RowItem(4)
        }),
    });
    layoutManager.setLayout(layout, this);

    // Initialize labels
    Array<Label*> labels = { &bpmLabel, &playLabel };
    for (Label* label : labels)
    {
        label->setColour(Label::textColourId, Colour(0xff000000));
    }

    //Initialize BPM field
    bpmEditor.setColour(TextEditor::textColourId, Colour(0xff000000));
    bpmEditor.setColour(TextEditor::outlineColourId, Colour(0xff000000));
    bpmEditor.setColour(TextEditor::backgroundColourId, Colour(0xffffffff));
    bpmEditor.setJustification(Justification::centred);

    bpmEditor.setInputRestrictions(3, "0123456789");
    bpmEditor.setText("60");


    // Initialize NavButtons
    Array<Widgets::NavButton*> navButtons = 
    {
        &upButton,
        &downButton,
        &playButton
    };
    for (Widgets::NavButton* button : navButtons)
    {
        button->setAlwaysOnTop(true);
        button->addListener(this);
    }

    // Initialize TextButtons
    Array<TextButton*> textButtons =
    {
        &saveButton,
        &loadButton,
        &clearButton
    };
    for (TextButton* button : textButtons)
    {
        button->addListener(this);
        button->setColour(TextButton::textColourOnId, Colour(0xff000000));
        button->setColour(TextButton::textColourOffId, Colour(0xff000000));
        button->setColour(TextButton::buttonColourId, Colour(0xffffffff));
    }
    
    // Initialize the first two music strips.
    musicStrips.add(new MusicStrip(true));
    musicStrips.add(new MusicStrip(false));
    for (MusicStrip* strip : musicStrips)
    {
        addAndMakeVisible(strip);
        strip->addListener(this);
    }

    addAndMakeVisible(noteGrid);
}


// Passes all page button actions on to the appropriate functions.
void ScrollingPage::buttonClicked(Button* button)
{
    std::map<Button*, std::function<void()>> actionMap =
    {
        { &upButton,    [this]() { changeStripPosition(-0.25); } },
        { &downButton,  [this]() { changeStripPosition(0.25); } },
        { &clearButton, [this]() { clearData(); } },
        { &saveButton,  [this]() { saveData(); } },
        { &loadButton,  [this]() { loadData(); } }, 
        {
            &playButton, [this]() 
            { 
                if (!playbackTimer.isTimerRunning())
                {
                    startPlayback(); 
                }
                else
                {
                    playbackTimer.stopPlayback();
                    scrollToCurrentPos();
                }
            }
        },
    };
    actionMap[button]();
}


// Toggles a note in the NoteGrid when it is clicked in a MusicStrip.
void ScrollingPage::noteClicked(MusicStrip* strip, const int note,
        const int beat) 
{
    const int stripIndex = musicStrips.indexOf(strip);
    DBG("Clicked note " << note << ", beat " << beat << " in strip "
            << stripIndex);
    if (stripIndex < 0)
    {
        jassertfalse;
        return;
    }
    int globalBeat = beat;
    for (int i = 0; i < stripIndex; i++)
    {
        globalBeat += musicStrips[i]->getBeatCount();
    }
    if (noteGrid.toggleNote(note, globalBeat))
    {
        notePlayer.playNote(note);
    }
}


// Plays back all notes in the music strip sequence.
void ScrollingPage::startPlayback()
{
    int bpm = bpmEditor.getText().getIntValue();
    if (bpm <= 0)
    {
        bpm = 60;
    }
    // Move to the start of the strip sequence:
    float cachedStripNum = stripNum;
    stripNum = 0;
    resized();
    stripNum = cachedStripNum;

    // Assemble playback data:
    std::map<int, Array<int>> noteMap = noteGrid.getNoteMap();
    int beatCount = noteGrid.getBeatCount();
    if (beatCount == 0)
    {
        DBG("No notes found, skipping playback");
        return;
    }
    beatCount++;

    // Start strip animation:
    const int yOffset = musicStrips[0]->getBeatHeight() * beatCount;
    const int scrollTime = 36000 / bpm * (beatCount + 1);
    for (MusicStrip* strip : musicStrips)
    {
        Layout::Transition::Animator::transformBounds(strip,
                strip->getBounds().translated(0, -yOffset), scrollTime);
    }
    Layout::Transition::Animator::transformBounds(&noteGrid,
            noteGrid.getBounds().translated(0, -yOffset), scrollTime, false,
            [this](){ scrollToCurrentPos(); });

    playbackTimer.startPlayback(&noteGrid, bpm);
}


// Applies the page layout when the component's bounds change.
void ScrollingPage::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    for (int i = 0; i < musicStrips.size(); i++)
    {
        musicStrips[i]->setBounds(getStripBounds(i));
    }
    int gridX = musicStrips[0]->getNoteX(0);
    int gridY = musicStrips[0]->getBeatTop() 
            - musicStrips[0]->getY() - NoteGrid::gridOffset;
    int gridWidth = getWidth() - gridX;
    int gridHeight = musicStrips.getLast()->getBottom() - gridY;
    noteGrid.setBounds(gridX, gridY, gridWidth, gridHeight);
    noteGrid.setScale(musicStrips[0]->getNoteWidth(),
            musicStrips[0]->getBeatHeight());
}


// Draws the playback bar over the music strip.
void ScrollingPage::paint(Graphics& g)
{
    if (stripNum == 0 || playbackTimer.isTimerRunning())
    {
        const int xPos = musicStrips[0]->getNoteX(0) 
                - musicStrips[0]->getNoteWidth();
        const int yPos = musicStrips[0]->getBeatTop();
        const int width = musicStrips[0]->getWidth() - 2 * xPos;
        const int height = musicStrips[0]->getBeatHeight() / 3;
        g.setColour(Colour(0x55996600));
        g.fillRect(Rectangle<int>(xPos, yPos, width, height));
    }
}


// Gets the bounds relative to the component where a MusicStrip should be drawn.
Rectangle<int> ScrollingPage::getStripBounds(const int index) const
{
    return Rectangle<int>(0, (index - stripNum) * getHeight(), upButton.getX(),
            getHeight());
}


// Scrolls the page forwards or backwards, animating the component transition.
void ScrollingPage::changeStripPosition(const float offset)
{
    // Don't use scroll buttons when scrolling or playing:
    if (Layout::Transition::Animator::isAnimating(&noteGrid))
    {
        return;
    }

    // Don't allow the offset to scroll to values less than zero:
    float newPos = std::max<float>(0, offset + stripNum);
    if (newPos == stripNum)
    {
        return;
    }
    stripNum = newPos;

    // Create a new strips if scrolling towards the bottom:
    if (stripNum >= (musicStrips.size() - 2))
    {
        musicStrips.add(new MusicStrip(false));
        musicStrips.getLast()->addListener(this);
        addAndMakeVisible(musicStrips.getLast());
        Rectangle<int> newStripBounds = getStripBounds(musicStrips.size());
        musicStrips.getLast()->setBounds(newStripBounds);
        noteGrid.setBounds(noteGrid.getBounds().withBottom(
                    newStripBounds.getBottom()));
    }

    // Scroll components to match the new position:
    scrollToCurrentPos();
}


// Scrolls the music strip sequence to the appropriate location for its current
// index.
void ScrollingPage::scrollToCurrentPos()
{
    const int newTop = musicStrips[0]->getBeatTop()
            + getStripBounds(0).getY() - NoteGrid::gridOffset;
    for (int i = 0; i < musicStrips.size(); i++)
    {
        Layout::Transition::Animator::transformBounds(musicStrips[i],
                getStripBounds(i), animationMS);
    }
    Layout::Transition::Animator::transformBounds(&noteGrid,
            noteGrid.getBounds().withY(newTop), animationMS);
}


// Clears all song data after getting confirmation from the user.
void ScrollingPage::clearData()
{
    if (Layout::Transition::Animator::isAnimating(&noteGrid)
            || noteGrid.isEmpty())
    {
        return;
    }
    Windows::Alert::confirmAction("Clear all data?",
            "This will erase the current song.", [this]()
    {
        noteGrid.clear();
        repaint();
        changeStripPosition(-stripNum); 
    });
}


// Saves all song data to a file selected by the user.
void ScrollingPage::saveData()
{
    MusicFile file = MusicFile::openFile(false);
    if (file.validFile())
    {
        file.setBPM(bpmEditor.getText().getIntValue());
        file.importNoteGrid(&noteGrid);
        file.writeToFile();
    }
}


// Loads all song data from a file selected by the user.
void ScrollingPage::loadData()
{
    MusicFile file = MusicFile::openFile(true);
    if (file.validFile())
    {
        noteGrid.clear();
        DBG("Found bpm = " << file.getBPM());
        bpmEditor.setText(String(file.getBPM()));
        file.exportToNoteGrid(&noteGrid);

        const int beatCount = noteGrid.getBeatCount();
        int beatSpace = 0;
        for(MusicStrip* musicStrip : musicStrips)
        {
            beatSpace += musicStrip->getBeatCount();
        }
        while (beatSpace <= beatCount)
        {
            musicStrips.add(new MusicStrip(false));
            addAndMakeVisible(musicStrips.getLast());
            beatSpace += musicStrips.getLast()->getBeatCount();
        }
        resized();
        changeStripPosition(-stripNum);
    }
}
