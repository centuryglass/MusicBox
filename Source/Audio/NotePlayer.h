/**
 * @file  NotePlayer.h
 *
 * @brief  Plays music box notes using audio samples.
 */

#pragma once
#include "JuceHeader.h"

class NotePlayer : public AudioSource
{
public:
    NotePlayer(AudioDeviceManager& deviceManager);
    virtual ~NotePlayer();

    void playNote(int notes);

    void playNotes(Array<int> notes);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

private:
    AudioDeviceManager& deviceManager;
    AudioFormatManager formatManager;
    OwnedArray<AudioTransportSource> transportSources;
    OwnedArray<AudioFormatReaderSource> noteSources;
    Array<double> sampleRates;
};
