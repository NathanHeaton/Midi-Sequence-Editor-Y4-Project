#pragma once
#include "ToolbarGroup.h"
#include "../../Singletons/ToolManager.h"
#include "../../Singletons/PlayBackManager.h"
#include "../../Singletons/ViewState.h"
#include "../../SetupAssets.h"

namespace ToolbarGroups {

    inline ToolbarGroup pianoRollTools() {
        return {
            "piano_roll_tools",
            []() {
                if (ImGui::ImageButton("edit_button",
                    (void*)(intptr_t)ASSETS.editIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setPianoRollTool(EDIT);
                ImGui::SameLine();

                if (ImGui::ImageButton("box_select",
                    (void*)(intptr_t)ASSETS.boxSelect.textureID, ImVec2(32,32)))
                    ToolManager::instance().setPianoRollTool(SELECT);
                ImGui::SameLine();

                if (ImGui::ImageButton("delete_button",
                    (void*)(intptr_t)ASSETS.deleteIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setPianoRollTool(DELETE);
                ImGui::SameLine();

                if (ImGui::ImageButton("move_tool",
                    (void*)(intptr_t)ASSETS.moveIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setPianoRollTool(MOVE);
                ImGui::SameLine();

                if (ImGui::ImageButton("cut_button",
                    (void*)(intptr_t)ASSETS.cutIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setPianoRollTool(SNIP);
            }
        };
    }

    inline ToolbarGroup arrangerTools() {
        return {
            "arranger_tools",
            []() {
                if (ImGui::ImageButton("arr_edit",
                    (void*)(intptr_t)ASSETS.editIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setArrangerTool(EDIT);
                ImGui::SameLine();

                if (ImGui::ImageButton("arr_move",
                    (void*)(intptr_t)ASSETS.moveIcon.textureID, ImVec2(32,32)))
                    ToolManager::instance().setArrangerTool(MOVE);
            }
        };
    }

    inline ToolbarGroup playback() {
        return {
            "playback",
            []() {
                auto& pb = PlayBackManager::instance();
                void* icon = !pb.isPlaying()
                    ? (void*)(intptr_t)ASSETS.playIcon.textureID
                    : (void*)(intptr_t)ASSETS.pauseIcon.textureID;

                if (ImGui::ImageButton("play_button", icon, ImVec2(32,32)))
                    pb.togglePlay();
            }
        };
    }

    inline ToolbarGroup pianoRollZoom() {
        return {
            "piano_roll_zoom",
            []() {
                if (ImGui::ImageButton("zoom_in",
                    (void*)(intptr_t)ASSETS.zoomIn.textureID, ImVec2(32,32)))
                    zoomFactor::pianoRoll += zoomFactor::pianoRoll * 0.15f;
                ImGui::SameLine();

                if (ImGui::ImageButton("zoom_out",
                    (void*)(intptr_t)ASSETS.zoomOut.textureID, ImVec2(32,32)))
                    zoomFactor::pianoRoll -= zoomFactor::pianoRoll * 0.15f;
            }
        };
    }

    inline ToolbarGroup arrangerZoom() {
        return {
            "arranger_zoom",
            []() {
                if (ImGui::ImageButton("arr_zoom_in",
                    (void*)(intptr_t)ASSETS.zoomIn.textureID, ImVec2(32,32)))
                    zoomFactor::arranger += zoomFactor::arranger * 0.15f;
                ImGui::SameLine();

                if (ImGui::ImageButton("arr_zoom_out",
                    (void*)(intptr_t)ASSETS.zoomOut.textureID, ImVec2(32,32)))
                    zoomFactor::arranger -= zoomFactor::arranger * 0.15f;
            }
        };
    }

    inline ToolbarGroup snapping() {
        return {
            "snapping",
            []() {
                if (ImGui::ImageButton("snapping",
                    (void*)(intptr_t)ASSETS.divisionSnapIcon.textureID, ImVec2(32,32)))
                    ImGui::OpenPopup("SnapPopup");

                if (ImGui::BeginPopup("SnapPopup")) {
                    static const char* snapOptions[] = {
                        "beat","1/2 beat","1/3 beat","1/4 beat","1/5 beat",
                        "1/6 beat","1/8 beat","1/12 beat","1/16 beat",
                        "1/24 beat","1/32 beat","free place"
                    };
                    for (int i = 0; i < IM_ARRAYSIZE(snapOptions); i++)
                        if (ImGui::Selectable(snapOptions[i]))
                            ViewState::instance().setSnapSubDivisions(i);
                    ImGui::EndPopup();
                }
            }
        };
    }
}