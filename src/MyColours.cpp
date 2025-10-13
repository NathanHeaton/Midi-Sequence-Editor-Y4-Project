#include "MyColours.h"

juce::Colour MyColours::background;
juce::Colour MyColours::textPri;
juce::Colour MyColours::textSce;
juce::Colour MyColours::backgroundAlt;
juce::Colour MyColours::accent;
juce::Colour MyColours::outline;

void MyColours::setDefaultTheme()
{
    background = juce::Colour::fromRGB(9,16,29);
    backgroundAlt = juce::Colour::fromRGB(19,27,42);        
    outline = juce::Colour::fromRGB(187,196,213);
    accent = juce::Colour::fromRGB(243,180,64);


    textPri = juce::Colour::fromRGB(255,255,255);
    textSce = juce::Colour::fromRGB(160,169,100);

}