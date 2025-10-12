#include "MyColours.h"

juce::Colour MyColours::background;
juce::Colour MyColours::textPri;
juce::Colour MyColours::textSce;
juce::Colour MyColours::backgroundAlt;

void MyColours::setDefaultTheme()
{
    background = juce::Colour::fromRGB(9,16,29);
    textPri = juce::Colour::fromRGB(19,16,29);
    textSce = juce::Colour::fromRGB(59,16,29);
    backgroundAlt = juce::Colour::fromRGB(96,16,29);        
}