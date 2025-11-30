#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "../Track/PatternViewport.h"
#include "../Track/HeadingComponent.h"
#include "../../MIDI_Logic/MidiFileManager.h"
#include "../../SessionData.h"

#include <iostream>


class ControlComponent: public juce::Component,
                        public juce::Button::Listener,
                        public juce::ActionBroadcaster,
public juce::TextEditor::Listener
{
public:

    MidiFileManager midifileManager;
    juce::TextButton openFileButton { "open Midi"};
    juce::TextButton createPatternButton { "create pattern"};
    juce::TextEditor barWidthInput;



    ControlComponent() {
        addAndMakeVisible(&barWidthInput);
        addAndMakeVisible(openFileButton);
        addAndMakeVisible(createPatternButton);

        barWidthInput.setMultiLine(false);
        barWidthInput.setJustification(juce::Justification::centredLeft);
        barWidthInput.setCaretVisible(true);
        barWidthInput.setText("200");
        barWidthInput.setInputRestrictions(3,"0123456789.");

        openFileButton.addListener(this);
        createPatternButton.addListener(this);
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
        g.drawText ("m_text", getLocalBounds(), juce::Justification::centred, true);

    }

    void textEditorReturnKeyPressed(juce::TextEditor &editor) override {
        if (&editor == &barWidthInput) {
            float width = barWidthInput.getText().getFloatValue();
            SessionData::instance().setBarWidth(width);
        }

    }



    void resized() override{
        juce::FlexBox row;
        row.flexDirection = juce::FlexBox::Direction::row;
        row.alignItems = juce::FlexBox::AlignItems::flexStart;
        row.items.add(juce::FlexItem (openFileButton).withHeight(40).withWidth(100));
        row.items.add(juce::FlexItem (createPatternButton).withHeight(40).withWidth(100));
        row.items.add(juce::FlexItem (barWidthInput).withHeight(40).withWidth(100));
        row.performLayout(getLocalBounds());
    }

    void buttonClicked(juce::Button* button) override {
        DBG("buttonClicked");
        if(button == &openFileButton){
            openMidiFilePicker();
        }
        else if(button == &createPatternButton) {
            DBG("sending");
            juce::ActionBroadcaster::sendActionMessage("open piano roll");
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
