#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
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
    size_t selectedPattern{0};
    ControlComponent() {
        SessionData::instance().addPattern();
    };

    void ControlPanel(auto &state) {
        auto& patterns = SessionData::instance().getPatterns();
        if (ImGui::BeginTable("Control Panel",4)) {
            ImGui::TableSetupColumn("Master",ImGuiTableColumnFlags_WidthFixed,200);
            ImGui::TableSetupColumn("Pattern",ImGuiTableColumnFlags_WidthFixed, 400);
            ImGui::TableSetupColumn("Playback",ImGuiTableColumnFlags_WidthFixed);

            ImGui::TableNextColumn();
            ImGui::Text("Master");
            ImGui::SliderFloat("",&test_vol,0.0f,1.0f);
            ImGui::TableNextColumn();

            if (ImGui::Button("New Pattern")) {
                state.pianoRollWindow = !state.pianoRollWindow;
                SessionData::instance().addPattern();
            }
            if (ImGui::BeginCombo("Open Pattern",
                patterns.at(selectedPattern).m_title.c_str()))
                {
                for (size_t i{0}; i < patterns.size(); i++) {
                    bool is_selected = (selectedPattern == i);
                    if (ImGui::Selectable(patterns.at(i).m_title.c_str(), is_selected)) {
                        selectedPattern = i;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        SessionData::instance().setCurrentPattern(i);
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("load midi")) {
                openMidiFilePicker();
            }

            ImGui::TableNextColumn();
            ImGui::Text("playback controls");
        }ImGui::EndTable();

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
