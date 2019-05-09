#include "ScrollingPage.h"
#include "Layout_Transition_Animator.h"

static const constexpr int animationMS = 200;


// Initializes the component layout and creates the first music strips.
ScrollingPage::ScrollingPage():
upButton(Widgets::NavButton::WindowEdge::up),
downButton(Widgets::NavButton::WindowEdge::down)
{
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(1,
        { 
            RowItem(8),
            RowItem(&upButton),
            RowItem(1)
        }),
        Row(5),
        Row(1,
        { 
            RowItem(8),
            RowItem(&downButton),
            RowItem(1)
        }),
    });
    layoutManager.setLayout(layout, this);
    upButton.setAlwaysOnTop(true);
    downButton.setAlwaysOnTop(true);
    upButton.addListener(this);
    downButton.addListener(this);
    
    musicStrips.add(new MusicStrip(true));
    musicStrips.add(new MusicStrip(false));
    addAndMakeVisible(musicStrips[0]);
    addAndMakeVisible(musicStrips[1]);
}


// Scrolls the list when the navigation buttons are clicked, creating new music
// strips when approaching the bottom edge.
void ScrollingPage::buttonClicked(Button* navButton)
{
    if (Layout::Transition::Animator::isAnimating(this))
    {
        DBG("Still animating, ignoring button click.");
        return;
    }
    if (navButton == &upButton)
    {
        if (stripNum == 0)
        {
            return;
        }
        stripNum -= 0.25;
    }
    else if (navButton == &downButton)
    {
        stripNum += 0.25;
    }
    if (stripNum >= (musicStrips.size() - 2))
    {
        musicStrips.add(new MusicStrip(false));
        addAndMakeVisible(musicStrips.getLast());
        musicStrips.getLast()->setBounds(getStripBounds(musicStrips.size()));
    }
    for (int i = 0; i < musicStrips.size(); i++)
    {
        Layout::Transition::Animator::transformBounds(musicStrips[i],
                getStripBounds(i), animationMS);
    }
}


// Applies the page layout when the component's bounds change.
void ScrollingPage::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    for (int i = 0; i < musicStrips.size(); i++)
    {
        musicStrips[i]->setBounds(getStripBounds(i));
    }
}


// Gets the bounds relative to the component where a MusicStrip should be drawn.
Rectangle<int> ScrollingPage::getStripBounds(const int index) const
{
    return Rectangle<int>(0, (index - stripNum) * getHeight(), upButton.getX(),
            getHeight());
}
