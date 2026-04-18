#pragma once
#include "Panels/PianoRollComponent.h"
#include <iostream>
#include <imgui.h>
#include "Panels/piano.h"
#include "../Common/TimelineLabel.h"
#include "Panels/velocity.h"
#include "Panels/velocityLabel.h"
#include "../Common/Toolbar.h"
#include "../Common/ToolbarGroups.h"
#include "../../Singletons/PanelManager.h"

enum class NoteExtraPanel { None, Velocity, Pan };

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

    float timelineXScroll  = 0.0f;
    float timelineLength   = ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll) * 8;
    float pianoRollScrollY = 300.0f;
    float pianoRollScrollX = 0.0f;
    float pianoRollHeight  = 120.0f;
    float m_availY         = 0.0f;

    float velocityHeight        = 60.0f;
    float m_bottomSectionHeight = 85.0f;
    float m_tabBarHeight        = 25.0f;
    float m_dragBarHeight       = 6.0f;

    const float VELOCITY_MAX_Y  = 200.0f;
    const float m_minVelHeight  = 15.0f;

    NoteExtraPanel activePanel = NoteExtraPanel::Velocity;
    bool initialLoad = true;

    PianoRollMain(PatternPanelState& panel) : panelDetails(&panel) {
        pianoRollToolbar.addGroup(ToolbarGroups::snapping(false));
        pianoRollToolbar.addGroup(ToolbarGroups::pianoRollTools());
        pianoRollToolbar.addPlayBackButton(ToolbarGroups::playback(false, panelDetails->patternID));
        pianoRollToolbar.addGroup(ToolbarGroups::pianoRollZoom());
    }

    void create() {
        auto patternTitle = PatternManager::instance().getPatternByID(panelDetails->patternID)->m_title;
        if (ImGui::Begin(patternTitle.c_str(), &panelDetails->open, ImGuiWindowFlags_NoScrollbar)) {

            if (initialLoad) {
                ImGui::SetNextWindowPos(panelDetails->pos);
                ImGui::SetNextWindowSize(panelDetails->size);
            }

            pianoRollToolbar.create();
            renderTimelineHeader();

            if (ImGui::BeginTable("note_edit_elements", 3, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano");
                ImGui::TableSetupColumn("sequence grid", ImGuiTableColumnFlags_WidthStretch, 0);
                ImGui::TableSetupColumn("verticalScroll", ImGuiTableColumnFlags_WidthFixed, 15);

                ImGui::TableNextColumn();
                piano.create(pianoRollScrollY, pianoRollHeight);

                ImGui::TableNextColumn();
                if (initialLoad || m_availY != ImGui::GetContentRegionAvail().y) {
                    m_availY = ImGui::GetContentRegionAvail().y;
                    pianoRollHeight = m_availY - m_bottomSectionHeight;
                }

                pianoRoll.create(pianoRollScrollY, timelineXScroll, timelineLength,
                    pianoRollHeight, *panelDetails);

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

            pianoRollHeight = m_availY - m_bottomSectionHeight;
            pianoRollHeight = std::max(pianoRollHeight, 120.0f);

            noteExtrasSection();
        }
        ImGui::End();
    }

private:
    void renderTimelineHeader() {
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
                PatternManager::instance().getCurrentPattern()->m_bars, false);

        } ImGui::EndTable();
    }

    void noteExtrasSection() {
        bool expanded = activePanel != NoteExtraPanel::None;

        if (expanded) {
            ImGui::Button("##drag_handle", ImVec2(ImGui::GetContentRegionAvail().x, m_dragBarHeight));
            if (ImGui::IsItemActive()) {
                velocityHeight -= ImGui::GetIO().MouseDelta.y;
                velocityHeight = std::clamp(velocityHeight, m_minVelHeight,
                    VELOCITY_MAX_Y - m_tabBarHeight - m_dragBarHeight);
            }
            m_bottomSectionHeight = velocityHeight + m_tabBarHeight + m_dragBarHeight;
        } else {
            m_bottomSectionHeight = m_tabBarHeight;
        }

        float tabBarTop = ImGui::GetCursorScreenPos().y;
        renderTabButtons();
        m_tabBarHeight = ImGui::GetCursorScreenPos().y - tabBarTop;

        if (expanded) {
            renderActivePanelContent();
        }
    }

    void renderTabButtons() {
        if (ImGui::BeginTable("Note Tabs", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableNextColumn();
            bool velocityActive = activePanel == NoteExtraPanel::Velocity;
            if (velocityActive) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            if (ImGui::Button("Velocity", ImVec2(80, 0))) {
                activePanel = velocityActive ? NoteExtraPanel::None : NoteExtraPanel::Velocity;
                if (activePanel == NoteExtraPanel::Velocity) velocityHeight = 60.0f;
            }
            if (velocityActive) ImGui::PopStyleColor();

            ImGui::TableNextColumn();
            bool panActive = activePanel == NoteExtraPanel::Pan;
            if (panActive) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            if (ImGui::Button("Pan", ImVec2(80, 0))) {
                activePanel = panActive ? NoteExtraPanel::None : NoteExtraPanel::Pan;
            }
            if (panActive) ImGui::PopStyleColor();

            ImGui::EndTable();
        }
    }

    void renderActivePanelContent() {
        if (activePanel == NoteExtraPanel::Velocity) {
            if (ImGui::BeginTable("velocity_elements", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("velocity Labels", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("vel grid", ImGuiTableColumnFlags_WidthStretch, 0);

                ImGui::TableNextColumn();
                velocityLabels.create();
                ImGui::TableNextColumn();
                velocity.create(timelineXScroll, panelDetails->patternID);

                ImGui::EndTable();
            }
        } else if (activePanel == NoteExtraPanel::Pan) {
            ImGui::Spacing();
            ImGui::TextDisabled("Pan editor coming soon...");
        }
    }
};