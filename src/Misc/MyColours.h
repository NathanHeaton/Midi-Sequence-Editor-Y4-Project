#pragma once 

#include <juce_gui_basics/juce_gui_basics.h>


class MyColours
{
    public:
        static juce::Colour background;
        static juce::Colour textPri;
        static juce::Colour textSce;
        static juce::Colour backgroundAlt;

        static void setDefaultTheme();
        
};

