#include "NotePlayer.h"
#include "Assets.h"

static const StringArray samples =
{
    "_177947__ubikphonik__ab1_aiff",
    "_177944__ubikphonik__bb1_aiff",
    "_177948__ubikphonik__c1_aiff",
    "_177950__ubikphonik__db1_aiff",
    "_177954__ubikphonik__eb1_aiff",
    "_177943__ubikphonik__f1_aiff",
    "_177941__ubikphonik__g1_aiff",
    "_177946__ubikphonik__ab2_aiff",
    "_177949__ubikphonik__bb2_aiff",
    "_177951__ubikphonik__c2_aiff",
    "_177952__ubikphonik__db2_aiff",
    "_177953__ubikphonik__eb2_aiff",
    "_177942__ubikphonik__f2_aiff",
    "_177940__ubikphonik__g2_aiff",
    "_177945__ubikphonik__ab3_aiff"
};

NotePlayer::NotePlayer(AudioDeviceManager& deviceManager) :
    deviceManager(deviceManager)
{

    formatManager.registerBasicFormats();
    for (const String& sample : samples)
    {
        File audioFile = Assets::findAssetFile(sample);
        AudioFormatReader* noteReader 
                = formatManager.createReaderFor(audioFile);
        if (noteReader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource 
                    = std::make_unique<AudioFormatReaderSource>(noteReader,
                    true);
            sampleRates.add(noteReader->sampleRate);
            noteSources.add(newSource.release());
            transportSources.add(new AudioTransportSource);
        }
        else
        {
            jassertfalse;
        }
    }
}

NotePlayer::~NotePlayer()
{
    transportSources.clear();
    noteSources.clear();
}

void NotePlayer::playNote(int note)
{
    const ScopedLock audioLock(deviceManager.getAudioCallbackLock());
    AudioTransportSource* transport = transportSources[note];
    AudioFormatReaderSource* noteSource = noteSources[note];
    if (transport != nullptr && noteSource != nullptr)
    {
        transport->setSource(noteSource, 0, nullptr, sampleRates[note]);
        transport->start();
    }
    else
    {
        DBG("Failed to start note " << note);
        jassertfalse;
    }
}

void NotePlayer::playNotes(Array<int> notes)
{
    for (const int& note : notes)
    {
        playNote(note);
    }
}

void NotePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    for (AudioTransportSource* source : transportSources)
    {
        if (source != nullptr)
        {
            source->prepareToPlay(samplesPerBlockExpected, sampleRate);
        }
    }
}

void NotePlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    const int numChannels = 2;
    const int numSamples = bufferToFill.numSamples;
    const int startSample = bufferToFill.startSample;
    bool sourcePlaying = false;
    for (AudioTransportSource* source : transportSources)
    {
        if (source != nullptr && source->isPlaying())
        {
            sourcePlaying = true;
            AudioBuffer<float> tempBuffer(numChannels, numSamples);
            AudioSourceChannelInfo tempInfo(&tempBuffer, startSample,
                    numSamples);
            source->getNextAudioBlock(tempInfo);
            for (int i = 0; i < numChannels; i++)
            {
                bufferToFill.buffer->addFrom(i, startSample, tempBuffer, i,
                        startSample, numSamples);
            }
        }
    }
    if(!sourcePlaying)
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

void NotePlayer::releaseResources()
{
    for (AudioTransportSource* source : transportSources)
    {
        if (source != nullptr)
        {
            source->releaseResources();
        }
    }
}
