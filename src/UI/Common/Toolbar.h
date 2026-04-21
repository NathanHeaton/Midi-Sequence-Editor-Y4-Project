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
        m_playBackActive = true;
        m_playBack = playBack;
    }
    void create() {

        int cols = m_playBackActive ? m_groups.size() + 1 : m_groups.size();
        ImGui::BeginTable("toolList", cols,
            ImGuiTableFlags_SizingFixedFit, ImVec2(0, 32));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        for (auto& group : m_groups) {
            ImGui::TableNextColumn();
                group.render(true);
        }
        if (m_playBackActive) {
            ImGui::TableNextColumn();
            m_playBack.renderP(true,0);
        }
        ImGui::PopStyleVar(2);
        ImGui::EndTable();

    }

private:
    std::vector<ToolbarGroup> m_groups;
    bool m_playBackActive = false;
    PlayBack m_playBack;
};