#include "Widgets_NavButton.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::NavButton::";
#endif


// Constructs the navigation button to sit on particular window edge.
Widgets::NavButton::NavButton(const WindowEdge edge) :
edge(edge)
{
    switch (edge)
    {
        case WindowEdge::up:
            setImage("upIcon_svg");
            break;
        case WindowEdge::down:
            setImage("downIcon_svg");
            break;
        case WindowEdge::left:
            setImage("leftIcon_svg");
            break;
        case WindowEdge::right:
            setImage("rightIcon_svg");
    }
    setWantsKeyboardFocus(false);
}


// Gets the window edge assigned to this NavButton.
Widgets::NavButton::WindowEdge Widgets::NavButton::getEdge() const
{
    return edge;
}
