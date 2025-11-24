//
// Created by nathan on 21/11/2025.
//

#include "../../Misc/MyColours.h"

#ifndef MYPROJECT_TRACKCONTENT_H
#define MYPROJECT_TRACKCONTENT_H

class TrackContent : public juce::Component
{
public:


    TrackContent()
    {
        setSize(3000, 150 );
    }

    void paint(juce::Graphics& g) override {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);


        //temp grid
        float const barWidth = 350.0f;
        bool AlternateBar = false;
        const int beatsPerBar = 4;
        float const tempDivision = float(barWidth / beatsPerBar); //changes this to match project time signature
        for (int i = 0; i < getWidth(); i+=barWidth) {
            if (AlternateBar) { g.setColour(MyColours::background);            }
            else {g.setColour(MyColours::backgroundAlt);}
            AlternateBar = !AlternateBar;
            g.fillRoundedRectangle(i,0,barWidth,getHeight(),0);
            g.setColour(MyColours::outline);
            g.drawLine(i, 0, i, getHeight(), 2);

            for (int j= 1; j< beatsPerBar; j++) {
                float Xposition = j * tempDivision + i;
                g.setColour(MyColours::altOutline);
                g.drawLine(Xposition, 0, Xposition, getHeight(), 1);
            }
        }
        //g.drawLine(i, 0, getHeight(), 50, 2);
    }

    void resized() override
    {

    }

private:

};

#endif //MYPROJECT_TRACKCONTENT_H