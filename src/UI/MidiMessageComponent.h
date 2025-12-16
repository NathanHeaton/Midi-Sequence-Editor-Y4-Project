#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Theme.h"

class MidiMessage : public juce::Component
{
public:
    MidiMessage()
    {
        // Configure the text editor
        textEditor.setMultiLine(true);
        textEditor.setReadOnly(true);
        textEditor.setScrollbarsShown(true);
        textEditor.setReturnKeyStartsNewLine(false);

        textEditor.setColour(juce::TextEditor::backgroundColourId, MyColours::backgroundAlt);
        textEditor.setColour(juce::TextEditor::outlineColourId, juce::Colours::white);
        textEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);

        addAndMakeVisible(textEditor);
    }

    void resized() override
    {
        textEditor.setBounds(getLocalBounds());
    }

    // Append a line of text to the log
    void addMessage(const juce::String& text)
    {
        textEditor.moveCaretToEnd();
        textEditor.insertTextAtCaret(text + "\n");

    }

private:
    juce::TextEditor textEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessage)
};
