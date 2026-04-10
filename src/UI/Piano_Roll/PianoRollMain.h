//
// Created by nathan on 24/11/2025.
//
#pragma once
#include "Panels/PianoRollComponent.h"
#include <iostream>
#include <imgui.h>
#include "Panels/piano.h"
#include "../Common/TimelineLabel.h"
#include "Panels/velocity.h"
#include  "Panels/velocityLabel.h"
#include "../Common/Toolbar.h"
#include "../Common/ToolbarGroups.h"
#include "../../Singletons/PanelManager.h"

class PianoRollMain
{
public:
    TimelineLabel timelineLabel;
    Piano piano;
    PianoRollComponent pianoRoll;
    Velocity velocity;
    VelocityLabels velocityLabels;
    Toolbar pianoRollToolbar;
    size_t patternIndex;

    PatternPanelState* panelDetails;

    float timelineXScroll = 0.0f;
    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll) * 8;

    PianoRollMain(PatternPanelState& panel) : panelDetails(&panel) {
        pianoRollToolbar.addGroup(ToolbarGroups::snapping(false));
        pianoRollToolbar.addGroup(ToolbarGroups::pianoRollTools());
        pianoRollToolbar.addGroup(ToolbarGroups::playback(false));
        pianoRollToolbar.addGroup(ToolbarGroups::pianoRollZoom());
    }

    float pianoRollScrollY = 300.0f;
    float pianoRollScrollX;
    bool initialLoad = true;

    float velocityHeight = 120.0f;
    const float VELOCITY_MAX_Y = 120.0f;
    float m_totalAvail{};
    float m_dragBarHeight = 6.0f;
    float m_minVelHeight  = 20.0f;

    void create() {
        if (initialLoad) {
            ImGui::SetNextWindowPos(panelDetails->pos);
            ImGui::SetNextWindowSize(panelDetails->size);
        }

        auto patternTitle = PatternManager::instance().getPatternByID(panelDetails->patternID)->m_title;
        if (ImGui::Begin(patternTitle.c_str(), &panelDetails->open,
            ImGuiWindowFlags_NoScrollbar)) {
            pianoRollToolbar.create();
            m_totalAvail = ImGui::GetContentRegionAvail().y;
            velocityHeight = std::clamp(velocityHeight, m_minVelHeight, m_totalAvail - 60.0f);

            if (ImGui::BeginTable("table", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("gap", ImGuiTableColumnFlags_WidthFixed,
                    ViewState::instance().getWhiteSize().x);
                ImGui::TableSetupColumn("timeline", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableNextColumn();
                ImGui::Dummy(ImVec2(ViewState::instance().getWhiteSize().x, 35));
                ImGui::TableNextColumn();
                if (ImGui::BeginChild("horizontalScroll", ImVec2(0, 15), false,
                    ImGuiWindowFlags_HorizontalScrollbar)) {
                    ImGui::Dummy(ImVec2(timelineLength, 15));
                    timelineXScroll = ImGui::GetScrollX();
                    } ImGui::EndChild();
                timelineLabel.create(timelineLength, timelineXScroll,
                    PatternManager::instance().getCurrentPattern()->m_bars,
                    false);
            }
            ImGui::EndTable();

            float pianoRollHeight = m_totalAvail - velocityHeight - m_dragBarHeight;

            if (ImGui::BeginTable("note_edit_elements", 3, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano");
                ImGui::TableSetupColumn("sequence grid", ImGuiTableColumnFlags_WidthStretch, 0);
                ImGui::TableSetupColumn("verticalScroll", ImGuiTableColumnFlags_WidthFixed, 15);

                ImGui::TableNextColumn();
                piano.create(pianoRollScrollY, pianoRollHeight);

                ImGui::TableNextColumn();
                pianoRoll.create(pianoRollScrollY,
                    timelineXScroll,
                    timelineLength,
                    pianoRollHeight,
                    panelDetails->patternID);

                ImGui::TableNextColumn();
                if (ImGui::BeginChild("custom_scroll", ImVec2(15, pianoRollHeight), false,
                    ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
                    ImGui::Dummy(ImVec2(15, static_cast<float>(ViewState::WHITE_KEYS)
                        * ViewState::instance().getWhiteSize().y));
                    if (initialLoad) {
                        ImGui::SetScrollY(ViewState::instance().getNoteHeight() * 48);
                        initialLoad = false;
                    }
                    pianoRollScrollY = ImGui::GetScrollY();
                    } ImGui::EndChild();
            } ImGui::EndTable();
            velocitySection();
            ImGui::End();
            }
    }

    void velocitySection() {
        // bool collapsed = (velocityHeight <= m_minVelHeight + 1.0f);
        //
        // const char* arrow = "Velocity";
        // if (ImGui::Button(arrow, ImVec2(120, m_dragBarHeight + 8))) {
        //     velocityHeight = collapsed
        //         ? 160.f   // expand to default
        //         : m_minVelHeight;           // collapse
        // }
        // ImGui::SameLine();
        // ImGui::Button("##drag_handle",
        //     ImVec2(ImGui::GetContentRegionAvail().x, m_dragBarHeight + 8));
        // if (ImGui::IsItemActive()) {
        //     velocityHeight -= ImGui::GetIO().MouseDelta.y;  // drag up = more height
        //     velocityHeight = std::clamp(velocityHeight, m_minVelHeight, m_totalAvail - 60.0f);
        // }

       // if (!collapsed) {
            if (ImGui::BeginTable("velocity_elements", 3, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano gap",
                    ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("vel grid",
                    ImGuiTableColumnFlags_WidthStretch, 0);

                ImGui::TableNextColumn();
                //piano.create(pianoRollScrollY, velocityHeight);
                velocityLabels.create(velocityHeight);
                ImGui::TableNextColumn();
                velocity.create(timelineXScroll);

            } ImGui::EndTable();

       // }
    }


    void updatePanelDetails() const {
        panelDetails->pos = ImGui::GetCursorPos();
        panelDetails->size = ImGui::GetContentRegionAvail();
    }


};

