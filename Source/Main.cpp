#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Assets.h"

class MusicBoxApplication  : public JUCEApplication
{
public:
    MusicBoxApplication() {}

    const String getApplicationName() override       
    {
        return ProjectInfo::projectName;
    }

    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    void initialise (const String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName()));
        mainWindow->setContentOwned(new MainComponent, true);
        mainWindow->setVisible(true);
    }

    void shutdown() override
    {
        Assets::clearTempCache();
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override { }

    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow(String name)  : DocumentWindow(name,
                Desktop::getInstance().getDefaultLookAndFeel().findColour(
                ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (MusicBoxApplication)
