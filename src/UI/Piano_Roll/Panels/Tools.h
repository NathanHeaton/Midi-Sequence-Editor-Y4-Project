#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Theme.h"
#include <iostream>
#include "../../../SetupAssets.h"
#include "../../../Singletons/PlayBackManager.h"

class Tools: public juce::Component
{
public:

    juce::Label label;

    Tools() = default;

    void create() {

        ImGui::Separator();
        ImGui::BeginTable("toolList",5,ImGuiTableFlags_SizingFixedFit,ImVec2(0,0));
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("play_button"
            , (void*)(intptr_t)ASSETS.playIcon.textureID
            , ImVec2(32, 32)
            )) {
            PlayBackManager::instance().togglePlay();
        }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("zoom_in"
            , (void*)(intptr_t)ASSETS.zoomIn.textureID
            , ImVec2(32, 32)
            )) {
            zoomFactor::pianoRoll += 0.15;
            }
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("zoom_out"
            , (void*)(intptr_t)ASSETS.zoomOut.textureID
            , ImVec2(32, 32)
            )) {
            zoomFactor::pianoRoll -= 0.15;
            }
        ImGui::TableNextColumn();
        ImGui::Text("d");
        ImGui::TableNextColumn();
        ImGui::Text("d");
        ImGui::EndTable();
        ImGui::Separator();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tools)
};
