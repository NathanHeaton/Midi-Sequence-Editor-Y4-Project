#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "../../../Theme.h"
#include <iostream>


class Tools: public juce::Component
{
public:

    juce::Label label;

    Tools() = default;

    void create() {

        ImGui::Separator();
        ImGui::BeginTable("toolList",5,ImGuiTableFlags_SizingFixedFit,ImVec2(0,0));
        ImGui::TableNextColumn();
        ImGui::Text("d");
        ImGui::TableNextColumn();
        ImGui::Text("d");
        ImGui::TableNextColumn();
        ImGui::Text("d");
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
