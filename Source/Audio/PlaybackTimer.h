#pragma once
#include "JuceHeader.h"
#include "NotePlayer.h"
#include <map>

class NoteGrid;

class PlaybackTimer : public Timer
{
public:
    PlaybackTimer(NotePlayer& notePlayer);

    virtual ~PlaybackTimer();

    void startPlayback(NoteGrid* notegrid, const int bpm);

    void stopPlayback();

private:
    void timerCallback() override;

    int msFromBPM(const int bpm);

    NotePlayer& notePlayer;
    NoteGrid* noteGrid = nullptr;
    int beatIndex = 0;
    int numBeats = 0;
    int frequency;
};
