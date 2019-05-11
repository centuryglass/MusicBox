#include "MainComponent.h"
#include "Windows_Info.h"
#include "Assets.h"

//==============================================================================
MainComponent::MainComponent() :
notePlayer(deviceManager),
scrollingPage(notePlayer)
{
    addAndMakeVisible(scrollingPage);
    Rectangle<int> displaySize = juce::Desktop::getInstance().getDisplays()
            .getMainDisplay().userArea;
    const int width = displaySize.getWidth() / 3;
    const int height = displaySize.getHeight() * 0.8;
    setBounds(0, 0, width, height);
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
            && ! RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
        [this](bool granted) 
        { 
            if (granted)
            {
                setAudioChannels (0, 1); 
            }
        });
    }
    else
    {
        setAudioChannels (0, 1);
    }
}

MainComponent::~MainComponent()
{ 
    shutdownAudio();
}

void MainComponent::prepareToPlay
(int samplesPerBlockExpected, double sampleRate)
{
    notePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock
(const AudioSourceChannelInfo& bufferToFill)
{
    notePlayer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    notePlayer.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.setColour(Colour(0xffffffff));
    g.fillRect(getLocalBounds());
}

void MainComponent::resized()
{
    scrollingPage.setBounds(getLocalBounds());
}
