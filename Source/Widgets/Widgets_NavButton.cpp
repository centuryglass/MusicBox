#include "Widgets_NavButton.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::NavButton::";
#endif


// Constructs the navigation button to sit on particular window edge.
Widgets::NavButton::NavButton(const WindowEdge edge) :
edge(edge)
{
    setColour(Widgets::DrawableImage::imageColour0Id, Colour(0xff000000));
    switch (edge)
    {
        case WindowEdge::up:
            setImage("NavButton/upIcon.svg");
            break;
        case WindowEdge::down:
            setImage("NavButton/downIcon.svg");
            break;
        case WindowEdge::left:
            setImage("NavButton/leftIcon.svg");
            break;
        case WindowEdge::right:
            setImage("NavButton/rightIcon.svg");
    }
    setWantsKeyboardFocus(false);
}


// Gets the window edge assigned to this NavButton.
Widgets::NavButton::WindowEdge Widgets::NavButton::getEdge() const
{
    return edge;
}
