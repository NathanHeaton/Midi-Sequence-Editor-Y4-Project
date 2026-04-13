//
// Created by nathan on 11/04/2026.

#include <juce_gui_extra/juce_gui_extra.h>
#include "TrackControls.h"
#include "../../Singletons/PlayBackManager.h"

void TrackControls::openAudioPicker() {
    juce::File initalDir = juce::File::getCurrentWorkingDirectory();
    auto chooser = std::make_shared<juce::FileChooser>
    ("choose a .WAV or .AIFF file",initalDir ,"*.WAV;*");
    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this, chooser](const juce::FileChooser& fc)
        {
            juce::File chosen = fc.getResult();
            if (chosen.existsAsFile())
            {
                DBG("Async selected file: " + chosen.getFullPathName());
                PlayBackManager::instance().addSoundFile(chosen);
            }
        });
}

void TrackControls::trackAudioControls() {
    if (ImGui::BeginTable("Controls",2)) {
        ImGui::TableNextColumn();
        auto instrumentID = ArrangerManager::instance().getTrack(m_index)->instrumentID;
        std::string instrumentName = PlayBackManager::instance().getInstrumentByID(instrumentID)->name;

        if (ImGui::BeginCombo("",instrumentName.c_str())) {
            auto list = PlayBackManager::instance().getInstruments();
            uint32_t selectedInstrument = 0;
            bool is_selected = false;
            for (auto& instrument : *list) {

                is_selected = (selectedInstrument == instrument->ID);
                if (ImGui::Selectable(instrument->name.c_str(), is_selected)) {
                    selectedInstrument = instrument->ID;
                    std::cout<<instrument->name.c_str()<<std::endl;
                    ArrangerManager::instance().setTrackInstrumentID(m_index, instrument->ID);
                    PlayBackManager::instance().getInstrumentByID(instrument->ID)->trackIndex = m_index;
                    ImGui::SetItemDefaultFocus();
                }
            }
            if (ImGui::Selectable("add more...", is_selected)) {
                openAudioPicker();
                std::cout<<"selecting"<<std::endl;
                selectedInstrument = list->back()->ID;
            }
            ImGui::EndCombo();
        }
        ImGui::TableNextColumn();
        ImGui::Text("vol");
    }ImGui::EndTable();

    if (ImGui::TableNextColumn()) {
        ImGui::Text("mute");
        ImGui::Text("solo");
    }
}
