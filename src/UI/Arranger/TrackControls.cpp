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
