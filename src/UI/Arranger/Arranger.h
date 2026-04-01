#pragma once

#include "../../Theme.h"
#include "TopControls.h"
#include "TrackControls.h"
#include "ArrTimeline.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/ProjectData.h"
#include "../../Singletons/ArrangerManager.h"
#include "../Common/timelineLabel.h"
#include <iostream>

#include "ArrTimeline.h"
#include "../Common/Toolbar.h"
#include "../Common/ToolbarGroups.h"
class Arranger
{
public:

    TopControls topControls;
    ArrTimeline arrangerTimeline;
    TimelineLabel timelineLabel;
    std::vector<std::unique_ptr<TrackControls>> tracksControls;
    Toolbar arrangerToolbar;
    const float trackControlWidth = 250.0f;

    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::arranger)* ProjectData::instance().getTotalBars();
    float timelineXScroll = 0.0f;
    Arranger() {
        AddTrack();
        arrangerToolbar.addGroup(ToolbarGroups::arrangerTools());
        arrangerToolbar.addGroup(ToolbarGroups::playback());
        arrangerToolbar.addGroup(ToolbarGroups::arrangerZoom());
    }


    void arranger() {
        ImGui::BeginChild("Arranger");

        if (ImGui::BeginTable("arranngerchild", 2)) {
            ImGui::TableSetupColumn("Top Controls", ImGuiTableColumnFlags_WidthFixed,trackControlWidth);
            ImGui::TableNextColumn();
            ImGui::Text("Arranger");
            ImGui::TableNextColumn();
            arrangerToolbar.create();
        }ImGui::EndTable();

        if (ImGui::BeginTable("Add and Scroll", 2)) {
            ImGui::TableSetupColumn("Top Controls", ImGuiTableColumnFlags_WidthFixed,trackControlWidth);
            ImGui::TableNextColumn();
            if (ImGui::Button("Add Track")) {
                AddTrack();
            }
            ImGui::TableNextColumn();
            if (ImGui::BeginChild("horizontalScroll", ImVec2(0, 15), false,
                ImGuiWindowFlags_HorizontalScrollbar)) {
                ImGui::Dummy(ImVec2(timelineLength, 15));
                timelineXScroll = ImGui::GetScrollX();
            } ImGui::EndChild();
        }ImGui::EndTable();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        if (ImGui::BeginTable("Track Timeline label", 2)) {
            ImGui::TableSetupColumn("Tracks1", ImGuiTableColumnFlags_WidthFixed, trackControlWidth);
            ImGui::TableNextColumn();
            ImGui::Text("Tracks");
            ImGui::TableNextColumn();

            timelineLabel.create(timelineLength,timelineXScroll,
                ProjectData::instance().getTotalBars(),
                zoomFactor::arranger);

        }ImGui::EndTable();
        ImGui::PopStyleVar();

        if (ImGui::BeginTable("Track Timline", 2)) {
            ImGui::TableSetupColumn("Tracks", ImGuiTableColumnFlags_WidthFixed, trackControlWidth);
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
            arrangerTimeline.createTimeline(timelineLength,timelineXScroll);
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
