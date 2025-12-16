//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include <iostream>
#include "imgui.h"

#ifndef MYPROJECT_TOPNAVCOMPONENT_H
#define MYPROJECT_TOPNAVCOMPONENT_H


class TopNavComponent
{
public:

    TopNavComponent() = default;

    void nav() {
        ImGui::BeginTable("Top Nav",3);
        ImGui::TableSetupColumn("File",ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Edit",ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Settings",ImGuiTableColumnFlags_WidthFixed);

        ImGui::TableNextColumn();
        ImGui::Text("File");
        ImGui::TableNextColumn();
        ImGui::Text("Edit");
        ImGui::TableNextColumn();
        ImGui::Text("Settings");

        ImGui::EndTable();
    }



    juce::StringArray fileOptions {"New", "Open","Open Recent", "Save", "Export","Import", "Quit"};
    juce::ComboBox editBox{"edit"};
    juce::ComboBox settingsBox{"settings"};
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopNavComponent)
};

#endif //MYPROJECT_TOPNAVCOMPONENT_H