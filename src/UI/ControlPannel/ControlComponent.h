#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../MIDI_Logic/MidiFileManager.h"
#include "imgui.h"
#include "../../Singletons/PanelManager.h"

class ControlComponent {
public:

    MidiFileManager midifileManager;

    float test_vol{0.0f};
    size_t selectedPattern{0};
    ControlComponent() {
        PatternManager::instance().addPattern();
    };

    void ControlPanel() {
        auto& patterns = PatternManager::instance().getPatterns();
        if (ImGui::BeginTable("Control Panel",4)) {
            ImGui::TableSetupColumn("Master",ImGuiTableColumnFlags_WidthFixed,200);
            ImGui::TableSetupColumn("Pattern",ImGuiTableColumnFlags_WidthFixed, 400);
            ImGui::TableSetupColumn("Playback",ImGuiTableColumnFlags_WidthFixed);

            ImGui::TableNextColumn();
            ImGui::Text("Master");
            ImGui::SliderFloat("",&test_vol,0.0f,1.0f);
            ImGui::TableNextColumn();

            if (ImGui::Button("New Pattern")) {
                PatternManager::instance().addPattern();
            }
            if (ImGui::BeginCombo("Select Pattern",
                patterns.at(selectedPattern).m_title.c_str()))
                {
                for (size_t i{0}; i < patterns.size(); i++) {
                    bool is_selected = (selectedPattern == i);
                    if (ImGui::Selectable(patterns.at(i).m_title.c_str(), is_selected)) {
                        selectedPattern = i;
                        PatternManager::instance().setCurrentPattern(i);
                        PatternManager::instance().setActivePatternID(PatternManager::instance().getCurrentPattern().ID);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("piano roll")) {
                PanelManager::instance().openPatern(PatternManager::instance().getCurrentPattern().ID);
            }
            if (ImGui::Button("import midi")) {
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
