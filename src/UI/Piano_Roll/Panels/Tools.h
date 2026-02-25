#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Theme.h"
#include <iostream>
#include "../../../SetupAssets.h"
#include "../../../Singletons/PlayBackManager.h"
#include "../../../Singletons/ToolManager.h"

class Tools: public juce::Component
{
public:

    juce::Label label;

    Tools() = default;

    void create() {

        ImGui::Separator();
        ImGui::BeginTable("toolList",12,ImGuiTableFlags_SizingFixedFit,ImVec2(0,0));
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("snapping",
            (void*)(intptr_t)ASSETS.divisionSnapIcon.textureID,
            ImVec2(32, 32))) {
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("edit_button",
            (void*)(intptr_t)ASSETS.editIcon.textureID,
            ImVec2(32, 32))) {
            ToolManager::instance().setNoteTool(EDIT);
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("box_select",
        (void*)(intptr_t)ASSETS.boxSelect.textureID,
        ImVec2(32, 32))) {
            ToolManager::instance().setNoteTool(SELECT);
        }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("delete_button",
            (void*)(intptr_t)ASSETS.deleteIcon.textureID,
            ImVec2(32, 32))) {
            ToolManager::instance().setNoteTool(DELETE);
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("move_tool",
            (void*)(intptr_t)ASSETS.moveIcon.textureID,
            ImVec2(32, 32))) {
            ToolManager::instance().setNoteTool(MOVE);
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("cut_button",
            (void*)(intptr_t)ASSETS.cutIcon.textureID,
            ImVec2(32, 32))) {
            ToolManager::instance().setNoteTool(SNIP);
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("play_button",
            !PlayBackManager::instance().isPlaying() ?
                (void*)(intptr_t)ASSETS.playIcon.textureID :
                (void*)(intptr_t)ASSETS.pauseIcon.textureID,
            ImVec2(32, 32))) {
            PlayBackManager::instance().togglePlay();
            }

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("zoom_in",
            (void*)(intptr_t)ASSETS.zoomIn.textureID,
            ImVec2(32, 32))) {
            zoomFactor::pianoRoll += pianoRoll * 0.15;
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("zoom_out",
            (void*)(intptr_t)ASSETS.zoomOut.textureID,
            ImVec2(32, 32))) {
            zoomFactor::pianoRoll -= pianoRoll * 0.15;
            }
        ImGui::TableNextColumn();
        ImGui::EndTable();
        ImGui::Separator();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tools)
};
