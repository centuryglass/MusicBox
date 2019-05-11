#include "PlaybackTimer.h"
#include "NoteGrid.h"

PlaybackTimer::PlaybackTimer(NotePlayer& notePlayer) :
notePlayer(notePlayer) { }

PlaybackTimer::~PlaybackTimer()
{
    if (isTimerRunning())
    {
        stopPlayback();
    }
}

void PlaybackTimer::startPlayback(NoteGrid* noteGrid, const int bpm)
{
    if (noteGrid == nullptr)
    {
        return;
    }
    if (isTimerRunning())
    {
        stopPlayback();
    }
    this->noteGrid = noteGrid;
    this->numBeats = noteGrid->getBeatCount();
    frequency = msFromBPM(bpm);
    startTimer(1);
}

void PlaybackTimer::stopPlayback()
{
    stopTimer();
    noteGrid = nullptr;
    beatIndex = 0;
    numBeats = 0;
}

void PlaybackTimer::timerCallback()
{
    stopTimer();
    if (beatIndex >= numBeats)
    {
        stopPlayback();
        return;
    }
    Array<int> notes = noteGrid->getNotes(beatIndex, true);
    if (! notes.isEmpty())
    {
        notePlayer.playNotes(notes);
    }
    beatIndex++;
    if (!isTimerRunning())
    {
        startTimer(frequency);
    }
}

int PlaybackTimer::msFromBPM(const int bpm)
{
    return 36000 / bpm;
}

