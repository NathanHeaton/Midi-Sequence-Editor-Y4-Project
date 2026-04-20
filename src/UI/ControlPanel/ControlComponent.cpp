//
// Created by nathan on 20/04/2026.
//
#include "ControlComponent.h"
#include "../../Singletons/PlayBackManager.h"

void ControlComponent::playbackControls() {

    playbackButtons.create();

    if (ImGui::BeginTable("##seek&time", 2, ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("##projectTimeline", ImGuiTableColumnFlags_WidthFixed, 200);
        ImGui::TableSetupColumn("##time");
        ImGui::TableNextColumn();
        ImGui::Dummy(ImVec2(200, 80));
        auto cursor = ImGui::GetCursorPos();
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(cursor.x,cursor.y),
            ImVec2(cursor.x + 200,cursor.y + 30),
                Theme::cTheme.backgroundPacked,
                3.0f
            );
        ImGui::TableNextColumn();
        std::string time = "1:00 / 1:20";
        ImGui::Text(time.c_str());
    }ImGui::EndTable();
}