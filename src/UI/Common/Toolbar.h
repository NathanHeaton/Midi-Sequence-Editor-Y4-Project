#pragma once
#include <vector>
#include "ToolbarGroup.h"
#include <imgui.h>

class Toolbar {
public:
    void addGroup(ToolbarGroup group) {
        m_groups.push_back(std::move(group));
    }


    void addPlayBackButton(PlayBack playBack) {
        m_playbackGroups.push_back(std::move(playBack));
    }
    void create() {

        int cols =m_playbackGroups.size() + m_groups.size();
        ImGui::BeginTable("toolList", cols,
            ImGuiTableFlags_SizingFixedFit, ImVec2(0, 32));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        for (auto& group : m_groups) {
            ImGui::TableNextColumn();
                group.render(true);
        }
        for (auto& group : m_playbackGroups) {
            ImGui::TableNextColumn();
            group.renderP(true,0);
        }
        ImGui::PopStyleVar(2);
        ImGui::EndTable();

    }

private:
    std::vector<ToolbarGroup> m_groups;
    std::vector<PlayBack>  m_playbackGroups;

};