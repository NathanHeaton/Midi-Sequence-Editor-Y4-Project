//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include "Panels/Tools.h"
#include "Panels/PianoRollComponent.h"
#include <iostream>
#include <imgui.h>
#include "Panels/piano.h"

class PianoRollMain : public juce::Component
{
public:
    Tools toolbar;
    Piano piano;

    PianoRollMain() = default;

    void create() {
        if (ImGui::Begin("pianoRollComponent", nullptr)) {
            toolbar.create();
            if (ImGui::BeginTable("table", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextColumn();
                piano.create();
                ImGui::TableNextColumn();
                ImGui::Text("PianoRoll");
            }ImGui::EndTable();

        }
        ImGui::End();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollMain)
};
