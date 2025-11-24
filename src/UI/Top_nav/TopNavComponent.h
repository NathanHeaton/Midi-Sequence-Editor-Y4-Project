//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include <iostream>

#ifndef MYPROJECT_TOPNAVCOMPONENT_H
#define MYPROJECT_TOPNAVCOMPONENT_H


class TopNavComponent : public juce::Component
{
public:

    TopNavComponent() {
        addAndMakeVisible(&filesBox);
        addAndMakeVisible(&editBox);
        addAndMakeVisible(&settingsBox);
        addAndMakeVisible(&label);
        label.setText("test",juce::dontSendNotification);
        label.setColour(0,MyColours::outline);
        filesBox.addItemList( fileOptions,1);
        //combo_boxes.push_back();

        filesBox.addSectionHeading("Files");
        editBox.addSectionHeading("edit");
        settingsBox.addSectionHeading("settings");

    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.alignItems = juce::FlexBox::AlignItems::flexStart;

        // CORRECTED SYNTAX: Add each item individually
        flex.items.add(juce::FlexItem(filesBox).withWidth(100).withHeight(25).withMargin(5)); // Increased width for visibility
        flex.items.add(juce::FlexItem(editBox).withWidth(100).withHeight(25).withMargin(5));
        flex.items.add(juce::FlexItem(settingsBox).withWidth(100).withHeight(25).withMargin(5));
        flex.items.add(juce::FlexItem(label).withWidth(100).withHeight(25).withMargin(5));

        // Alternative JUCE chain syntax using operator<<
        // flex.items << juce::FlexItem(filesBox).withWidth(100)
        //            << juce::FlexItem(editBox).withWidth(100)
        //            << juce::FlexItem(settingsBox).withWidth(100);

        flex.performLayout(getLocalBounds());
    }
    juce::Label label;

    juce::ComboBox filesBox{"file"};

    juce::StringArray fileOptions {"New", "Open","Open Recent", "Save", "Export","Import", "Quit"};
    juce::ComboBox editBox{"edit"};
    juce::ComboBox settingsBox{"settings"};
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopNavComponent)
};

#endif //MYPROJECT_TOPNAVCOMPONENT_H