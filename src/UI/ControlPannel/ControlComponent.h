#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "../Track/PatternViewport.h"
#include "../Track/HeadingComponent.h"
#include "../../MIDI_Logic/MidiFileManager.h"
#include "../../SessionData.h"
#include "imgui.h"

#include <iostream>


class ControlComponent {
public:

    MidiFileManager midifileManager;

    float test_vol{0.0f};
    ControlComponent() =default;

    void ControldPanel() {

        ImGui::BeginTable("Control Panel",4);
        ImGui::TableSetupColumn("Master",ImGuiTableColumnFlags_WidthFixed,200);
        ImGui::TableSetupColumn("Pattern",ImGuiTableColumnFlags_WidthFixed, 200);
        ImGui::TableSetupColumn("Playback",ImGuiTableColumnFlags_WidthFixed);

        ImGui::TableNextColumn();
        ImGui::Text("Master");
        ImGui::SliderFloat("",&test_vol,0.0f,1.0f);
        ImGui::TableNextColumn();
        if (ImGui::Button("create pattern")) {

        }
        if (ImGui::Button("load midi")) {
            openMidiFilePicker();
        }
        ImGui::TableNextColumn();
        ImGui::Text("playback controls");
        ImGui::EndTable();
    }


    void openMidiFilePicker() {
        juce::File initalDir = juce::File::getCurrentWorkingDirectory();
        auto chooser = std::make_shared<juce::FileChooser>
        ("choose a .midi or .mid file",initalDir ,"*.mid;*");
        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& fc)
            {
                juce::File chosen = fc.getResult();
                if (chosen.existsAsFile())
                {
                    DBG("Async selected file: " + chosen.getFullPathName());
                    midifileManager.loadFile(chosen);
                }
            });
    }
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlComponent)
};
