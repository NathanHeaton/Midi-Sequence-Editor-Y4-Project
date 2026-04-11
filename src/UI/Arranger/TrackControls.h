//
// Created by nathan on 09/12/2025.
//
#include "imgui.h"
#include "../../Singletons/ArrangerManager.h"
#ifndef MYPROJECT_TRACK_H
#define MYPROJECT_TRACK_H

class TrackControls {
public:
    TrackControls() = default;
    size_t m_index{};

    void newTrack(size_t trackIndex) {
        m_index=trackIndex;
        auto trackDetails = ArrangerManager::instance().getTrack(trackIndex);
        if (ImGui::BeginTable("Track",2,ImGuiTableFlags_SizingFixedFit
            |ImGuiTableFlags_BordersOuter
            , ImVec2(0, 100))) {
            ImGui::TableSetupColumn("Title",ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("muteSoloControls",ImGuiTableColumnFlags_WidthFixed, 50);
            if (ImGui::TableNextColumn()) {
                ImGui::Text(trackDetails->title.c_str());
                trackAudioControls();

            }ImGui::EndTable();
        }
    }
    void trackAudioControls();
    void openAudioPicker();
};
#endif //MYPROJECT_TRACK_H