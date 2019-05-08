/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
upButton(Widgets::NavButton::WindowEdge::up),
downButton(Widgets::NavButton::WindowEdge::down)
{
    setSize (600, 400);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(1,
        { 
            RowItem(3),
            RowItem(&upButton),
            RowItem(3)
        }),
        Row(5),
        Row(1,
        { 
            RowItem(3),
            RowItem(&downButton),
            RowItem(3)
        }),
    });
    layoutManager.setLayout(layout, this);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
}
