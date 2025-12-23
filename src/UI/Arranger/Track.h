//
// Created by nathan on 09/12/2025.
//
#include "imgui.h"

#ifndef MYPROJECT_TRACK_H
#define MYPROJECT_TRACK_H

class Track {
public:
    Track() = default;

    void newTrack() {

        if (ImGui::BeginTable("Track",2,ImGuiTableFlags_SizingFixedFit
            |ImGuiTableFlags_BordersOuter
            , ImVec2(0, 100))) {
            ImGui::TableSetupColumn("Title",ImGuiTableColumnFlags_WidthFixed, 300);
            ImGui::TableSetupColumn("muteSoloControls",ImGuiTableColumnFlags_WidthFixed, 100);
            if (ImGui::TableNextColumn()) {
                ImGui::Text("Track Title");
                ImGui::BeginTable("Controls",2);
                ImGui::TableNextColumn();
                ImGui::Button("Instrument");
                ImGui::TableNextColumn();
                ImGui::Text("vol");
            }ImGui::EndTable();
            if (ImGui::TableNextColumn()) {
                ImGui::Text("mute");
                ImGui::Text("solo");
            }
        }ImGui::EndTable();

    }
};
#endif //MYPROJECT_TRACK_H