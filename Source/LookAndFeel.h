/*
  ==============================================================================

    LookAndFeel.h
    Created: 8 May 2019 2:58:15pm
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

namespace MusicBox { class LookAndFeel; }

class MusicBox::LookAndFeel : public juce::LookAndFeel_V4
{
public:
LookAndFeel();

virtual ~LookAndFeel() { }
};
