#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include "TopControls.h"
#include "Track.h"
#include "Timeline.h"
#include "../../SessionData.h"
#include "../Common/timelineLabel.h"
#include <iostream>

#include "Timeline.h"


class Arranger
{
public:

    TopControls topControls;
    PatternViewport patternViewport;
    Timeline timeline;
    TimelineLabel timelineLabel;

    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<HeadingComponent>> trackComponents;


    float timelineLength = SessionData::instance().getBarWidth()* SessionData::instance().getTotalBars();
    float timelineXScroll = 0.0f;
    Arranger() {
        AddTrack();
    }


    void arranger() {
        ImGui::BeginChild("Arranger");

        if (ImGui::BeginTable("arranngerchild", 3)) {
            ImGui::TableSetupColumn("Top Controls", ImGuiTableColumnFlags_WidthFixed,300);
            ImGui::TableNextColumn();
            ImGui::Text("Arranger");
            ImGui::TableNextColumn();
            if (ImGui::Button("Add Track")) {
                AddTrack();
            }
        }ImGui::EndTable();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        if (ImGui::BeginTable("Track Timline label", 2)) {
            ImGui::TableSetupColumn("Tracks1", ImGuiTableColumnFlags_WidthFixed, 400);
            ImGui::TableNextColumn();
            ImGui::Text("Tracks");
            ImGui::TableNextColumn();

            timelineLabel.create(timelineLength,timelineXScroll,
                SessionData::instance().getTotalBars(),
                SessionData::instance().getBarWidth());

        }ImGui::EndTable();
        ImGui::PopStyleVar();
        if (ImGui::BeginTable("Track Timline", 2)) {
            ImGui::TableSetupColumn("Tracks", ImGuiTableColumnFlags_WidthFixed, 400);
            ImGui::TableSetupColumn("Timeline");

            ImGui::TableNextColumn();
            for (const auto& track : tracks) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                track->newTrack();
                ImGui::PopStyleVar();
            }

            ImGui::TableNextColumn();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            timeline.createTimeline(timelineLength,timelineXScroll);
            ImGui::PopStyleVar();
        }
        ImGui::EndTable();

        ImGui::EndChild();
    }




    void AddTrack()  {
        auto s = &SessionData::instance();
        DBG("adding track");
        auto newTrack =  std::make_unique<Track>(); // create a new track
        tracks.push_back(std::move(newTrack));
        s->addTrack();
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arranger)
};
