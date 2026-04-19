
#include "Arranger.h"
#include "../Common/TimelineLabel.h"

void Arranger::create() {
    if (ArrangerManager::instance().arrangerUpdated) {
        uint totalBars = (ArrangerManager::instance().getlastClipEndTime()/static_cast<float>(TimeData::PPQ)/
            static_cast<float>(TimeData::instance().timeSignature.getNumerator())) + 8;
        if (totalBars < 30) {totalBars = 30;}
        if (totalBars > ProjectData::instance().getTotalBars()) {
            ProjectData::instance().setTotalBars(totalBars);
            timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::arranger)*
                ProjectData::instance().getTotalBars();
        }

        ArrangerManager::instance().arrangerUpdated = false; // come up with better system if used in multiple spots
    }
        ImGui::BeginChild("Arranger");

        if (ImGui::BeginTable("arranger child", 2)) {
            ImGui::TableSetupColumn("Top Controls", ImGuiTableColumnFlags_WidthFixed,trackControlWidth);
            ImGui::TableNextColumn();
            ImGui::PushFont(Theme::textHeading);
            ImGui::PushStyleColor(ImGuiCol_Text, Theme::cTheme.outline);
            ImGui::Text("Arranger");
            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();
            arrangerToolbar.create();
        }ImGui::EndTable();

        if (ImGui::BeginTable("Add and Scroll", 2)) {
            ImGui::TableSetupColumn("Top Controls", ImGuiTableColumnFlags_WidthFixed,trackControlWidth);
            ImGui::TableNextColumn();

            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, Theme::borderSize);
            ImGui::PushFont(Theme::textMedium);
            if (ImGui::Button("Add Track")) {
                AddTrack();
            }
            ImGui::PopFont();
            ImGui::PopStyleVar();
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
                true);

        }ImGui::EndTable();
        ImGui::PopStyleVar();

        if (ImGui::BeginTable("Track Timline", 2)) {
            ImGui::TableSetupColumn("Tracks", ImGuiTableColumnFlags_WidthFixed, trackControlWidth);
            ImGui::TableSetupColumn("Timeline");

            ImGui::TableNextColumn();
            for (int i = 0; i < tracksControls.size(); i++) {
                tracksControls.at(i)->newTrack(i);
            }

            ImGui::TableNextColumn();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            arrangerTimeline.createTimeline(timelineLength,timelineXScroll);
            ImGui::PopStyleVar();
        }
        ImGui::EndTable();

        ImGui::EndChild();
    }

void Arranger::AddTrack()  {
    auto newTrack =  std::make_unique<TrackControls>(); // create a new track
    tracksControls.push_back(std::move(newTrack));
    ArrangerManager::instance().addTrack();
}
