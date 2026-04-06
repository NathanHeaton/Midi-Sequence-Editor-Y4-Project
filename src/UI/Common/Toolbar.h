#pragma once
#include <vector>
#include "ToolbarGroup.h"
#include <imgui.h>

class Toolbar {
public:
    void addGroup(ToolbarGroup group) {
        m_groups.push_back(std::move(group));
    }

    void create() {
        ImGui::Separator();

        ImGui::BeginTable("toolList", static_cast<int>(m_groups.size()),
            ImGuiTableFlags_SizingFixedFit, ImVec2(0, 0));

        for (auto& group : m_groups) {
            ImGui::TableNextColumn();
            group.render(true);
        }

        ImGui::EndTable();
        ImGui::Separator();
    }

private:
    std::vector<ToolbarGroup> m_groups;
};