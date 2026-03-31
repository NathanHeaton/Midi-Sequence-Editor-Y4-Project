#pragma once

#include "../../Theme.h"
#include "TopControls.h"
#include "TrackControls.h"
#include "Timeline.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/ProjectData.h"
#include "../../Singletons/ArrangerManager.h"
#include "../Common/timelineLabel.h"
#include <iostream>

#include "Timeline.h"
#include "../Common/Tools.h"

class Arranger
{
public:

    TopControls topControls;
    Timeline timeline;
    TimelineLabel timelineLabel;
    Tools tools;
    std::vector<std::unique_ptr<TrackControls>> tracksControls;

    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::arranger)* ProjectData::instance().getTotalBars();
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
            ImGui::TableNextColumn();
            tools.create();
        }ImGui::EndTable();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        if (ImGui::BeginTable("Track Timline label", 2)) {
            ImGui::TableSetupColumn("Tracks1", ImGuiTableColumnFlags_WidthFixed, 250);
            ImGui::TableNextColumn();
            ImGui::Text("Tracks");
            ImGui::TableNextColumn();

            timelineLabel.create(timelineLength,timelineXScroll,
                ProjectData::instance().getTotalBars(),
                zoomFactor::arranger);

        }ImGui::EndTable();
        ImGui::PopStyleVar();
        if (ImGui::BeginTable("Track Timline", 2)) {
            ImGui::TableSetupColumn("Tracks", ImGuiTableColumnFlags_WidthFixed, 250);
            ImGui::TableSetupColumn("Timeline");

            ImGui::TableNextColumn();
            for (int i = 0; i < tracksControls.size(); i++) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                tracksControls.at(i)->newTrack(i);
                ImGui::PopStyleVar();
            }
            for (const auto& track : tracksControls) {

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
        auto newTrack =  std::make_unique<TrackControls>(); // create a new track
        tracksControls.push_back(std::move(newTrack));
        ArrangerManager::instance().addTrack();
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arranger)
};
