//
// Created by nathan on 21/11/2025.
//

#ifndef MYPROJECT_TRACKCONTENT_H
#define MYPROJECT_TRACKCONTENT_H


class TrackContent : public juce::Component
{
public:


    TrackContent()
    {
        setSize(3000, 100 );
    }

    void paint(juce::Graphics& g) override {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);

        //temp grid
        for (int i =0; i < getWidth(); i+=50) {
            g.drawRoundedRectangle(i,0,50,getHeight(),0,1);
        }
    }

    void resized() override
    {

    }

private:

};

#endif //MYPROJECT_TRACKCONTENT_H