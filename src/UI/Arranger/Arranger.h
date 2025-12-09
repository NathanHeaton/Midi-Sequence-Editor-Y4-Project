#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "TopControls.h"
#include "Track.h"
#include <iostream>


class Arranger
{
public:

    TopControls topControls;
    PatternViewport patternViewport;

    std::unique_ptr<std::vector<Track>> track;

    // track controls
    int numTracks = 1;
    std::vector<std::unique_ptr<HeadingComponent>> trackComponents;

    
    Arranger() {
        AddTrack();
    }

    void arranger() {
        ImGui::Begin("Arranger");
        ImGui::End;
    }




    void AddTrack()  {
        DBG("adding track");
        auto newTrack = Track();
        track->push_back(newTrack);

    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arranger)
};
