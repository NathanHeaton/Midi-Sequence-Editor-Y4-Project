//
// Created by nathan on 09/12/2025.
//
#pragma once

#include "imgui.h"
#include "../../Theme.h"
#include "../../Singletons/ArrangerManager.h"
#include "../Common/StyledUIComponents.h"

class TrackControls {
public:
    TrackControls() = default;
    size_t m_index{};

    void newTrack(size_t trackIndex) {
        m_index=trackIndex;
        auto trackDetails = ArrangerManager::instance().getTrack(trackIndex);
        if (ImGui::BeginTable("##Track",2,ImGuiTableFlags_SizingFixedFit
            |ImGuiTableFlags_BordersOuter
            , ImVec2(0, 100))) {
            ImGui::TableSetupColumn("##Title",ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("##muteSoloControls",ImGuiTableColumnFlags_WidthFixed, 50);
            if (ImGui::TableNextColumn()) {
                UI::EditableTitle(trackDetails->title, "##title");
                //ImGui::Text(trackDetails->title.c_str());
                trackAudioControls();

            }ImGui::EndTable();
        }
    }
    void trackAudioControls();
    void openAudioPicker();
};
