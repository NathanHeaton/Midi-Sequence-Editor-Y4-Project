#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../MIDI_Logic/MidiFileManager.h"
#include "imgui.h"
#include "../../Theme.h"
#include "../../Singletons/PanelManager.h"
#include "../../Singletons/ProjectData.h"
#include "../Common/StyledUIComponents.h"

#include "../Common/Toolbar.h"
#include "../Common/ToolbarGroups.h"

class ControlComponent {
public:

    MidiFileManager midifileManager;
    Toolbar playbackButtons;

    size_t selectedPattern{0};
    ControlComponent() {
        PatternManager::instance().addPattern();
        playbackButtons.addPlayBackButton(ToolbarGroups::playback(true, 0));
    };
    float controlPanelHeight{80.0f};
    void ControlPanel() {

        if (ImGui::BeginTable("Control Panel",7,ImGuiActivateFlags_None, ImVec2(0,controlPanelHeight))) {
            ImGui::TableSetupColumn("Master",ImGuiTableColumnFlags_WidthFixed,200);
            ImGui::TableSetupColumn("seperator",ImGuiTableColumnFlags_WidthFixed,0);
            ImGui::TableSetupColumn("Pattern",ImGuiTableColumnFlags_WidthFixed, 220);
            ImGui::TableSetupColumn("seperator",ImGuiTableColumnFlags_WidthFixed,0);
            ImGui::TableSetupColumn("pianoRoll",ImGuiTableColumnFlags_WidthFixed,220);
            ImGui::TableSetupColumn("seperator",ImGuiTableColumnFlags_WidthFixed,0);
            ImGui::TableSetupColumn("Playback",ImGuiTableColumnFlags_WidthFixed);

            ImGui::TableNextColumn();
            ImGui::PushFont(Theme::textHeading);
            ImGui::Text("Master");
            ImGui::PopFont();
            ImGui::PushFont(Theme::monoSmall);
            ImGui::SliderFloat("##master slider", &ProjectData::instance().masterVolume, 0.0f,1.0f);
            ImGui::PopFont();

            ImGui::TableNextColumn();
            UI::VerticalSeparator(controlPanelHeight);
            ImGui::TableNextColumn();
            patternControls();
            ImGui::TableNextColumn();
            UI::VerticalSeparator(controlPanelHeight);
            ImGui::TableNextColumn();
            if (ImGui::Button("piano roll")) {
                PanelManager::instance().openPatern(PatternManager::instance().getCurrentPattern()->ID);
            }
            if (ImGui::Button("import midi")) {
                openMidiFilePicker();
            }
            ImGui::PopFont();
            ImGui::PopStyleVar(1);
            ImGui::TableNextColumn();
            UI::VerticalSeparator(controlPanelHeight);
            ImGui::TableNextColumn();
            playbackControls();

        }ImGui::EndTable();

    }

    void playbackControls();

    void patternControls() {
        auto& patterns = PatternManager::instance().getPatterns();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, Theme::borderSize);
        ImGui::PushFont(Theme::textMedium);
        if (ImGui::Button("New Pattern")) {
            PatternManager::instance().addPattern();
        }
        if (ImGui::BeginCombo("##pattern select",
            patterns.at(selectedPattern).m_title.c_str()))
        {
            for (size_t i{0}; i < patterns.size(); i++) {
                bool is_selected = (selectedPattern == i);
                if (ImGui::Selectable(patterns.at(i).m_title.c_str(), is_selected)) {
                    selectedPattern = i;
                    PatternManager::instance().setCurrentPattern(i);
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
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
