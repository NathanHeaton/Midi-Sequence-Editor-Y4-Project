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

    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<HeadingComponent>> trackComponents;

    
    Arranger() {
        AddTrack();
    }

    void arranger() {
        ImGui::Begin("Arranger");
        if (ImGui::BeginTable("arranngerchild",1))
        for (const auto& track : tracks) {
            track->newTrack();
        }

        ImGui::End();
    }




    void AddTrack()  {
        DBG("adding track");
        auto newTrack =  std::make_unique<Track>(); // create a new track
        tracks.push_back(std::move(newTrack));
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arranger)
};
