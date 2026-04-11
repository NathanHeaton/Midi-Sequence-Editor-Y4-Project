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

    void newTrack(size_t trackIndex) {

        auto trackDetails = ArrangerManager::instance().getTrack(trackIndex);
        if (ImGui::BeginTable("Track",2,ImGuiTableFlags_SizingFixedFit
            |ImGuiTableFlags_BordersOuter
            , ImVec2(0, 100))) {
            ImGui::TableSetupColumn("Title",ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("muteSoloControls",ImGuiTableColumnFlags_WidthFixed, 50);
            if (ImGui::TableNextColumn()) {
                ImGui::Text(trackDetails->title.c_str());
                ImGui::BeginTable("Controls",2);
                ImGui::TableNextColumn();
                if (ImGui::Button("Instrument")) {
                    openAudioPicker();
                };
                ImGui::TableNextColumn();
                ImGui::Text("vol");
            }ImGui::EndTable();
            if (ImGui::TableNextColumn()) {
                ImGui::Text("mute");
                ImGui::Text("solo");
            }
        }ImGui::EndTable();
    }

    void openAudioPicker();
};
#endif //MYPROJECT_TRACK_H