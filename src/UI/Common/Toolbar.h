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
        ImGui::Separator();

        ImGui::BeginTable("toolList", static_cast<int>(m_groups.size()),
            ImGuiTableFlags_SizingFixedFit, ImVec2(0, 0));

        for (auto& group : m_groups) {
            ImGui::TableNextColumn();
                group.render(true);
        }

        if (m_playBackActive) {
            m_playBack.renderP(true,0);
        }

        ImGui::EndTable();
        ImGui::Separator();
    }

private:
    std::vector<ToolbarGroup> m_groups;
    bool m_playBackActive = false;
    PlayBack m_playBack;
};