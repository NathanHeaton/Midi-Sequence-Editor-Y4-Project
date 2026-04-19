//
// Created by nathan on 19/04/2026.
//
#pragma once
#include <imgui.h>
#include "../../Theme.h"


namespace UI {
    inline void VerticalSeparator(float height = 0.0f, float thickness = 1.0f) {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        if (height == 0.0f) {
            height = ImGui::GetFrameHeight();
        }
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x, pos.y + height),
            Theme::cTheme.barColourPacked,
            thickness
        );
        ImGui::Dummy(ImVec2(thickness + 4.0f, height));
    }

    inline void EditableTitle(std::string& title, const char* id, ImFont* font = Theme::trackTitle) {
        ImGui::PushFont(font);
        static bool editing = false;
        static char buffer[256];

        if (editing) {
            ImGui::SetNextItemWidth(150.0f);
            if (ImGui::InputText(id, buffer, sizeof(buffer),
                ImGuiInputTextFlags_EnterReturnsTrue |
                ImGuiInputTextFlags_AutoSelectAll)) {
                title = std::string(buffer);
                editing = false;
                }
            if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(0)) {
                title = std::string(buffer);
                editing = false;
            }
        }
        else {
            ImGui::Text("%s", title.c_str());
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                editing = true;
                strncpy(buffer, title.c_str(), sizeof(buffer));
                ImGui::SetKeyboardFocusHere(-1);
            }
        }
        ImGui::PopFont();
    }

}