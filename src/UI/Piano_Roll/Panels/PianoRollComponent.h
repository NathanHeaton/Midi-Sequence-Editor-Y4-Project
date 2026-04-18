
//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/ViewState.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"
#include "../../../Singletons/PlayBackManager.h"
#include "PianoRollEventHandler.h"
#include "TimelineContext.h"
#include "../../../Singletons/PanelManager.h"

class PianoRollComponent {
public:

    PianoRollComponent() = default;
    ViewState* view_state = &ViewState::instance();
    bool bg_tone = false;
    PianoRollInputHandler inputHandler;
    uint32_t m_patternID{0};
    size_t m_trackIndex{0};

    void create(float &scrollY, float &scrollX, float& lengthX, float height, PatternPanelState panel_state) {
        if (ImGui::BeginChild("piano grid", ImVec2(0, height),
            ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            TimelineContext ctx;
            m_patternID = panel_state.patternID;
            m_trackIndex = panel_state.trackIndex;
            renderSteps(ctx);
            inputHandler.process(ctx, m_patternID, m_trackIndex);

            ImGui::SetScrollX(scrollX);
            ImGui::SetScrollY(scrollY);
            auto pattern = PatternManager::instance().getCurrentPattern();

            lengthX =  view_state->getPixelPerBar(pianoRoll) * (pattern->m_bars+1);// extra bar for visual purposes
            ImGui::Dummy(ImVec2(lengthX ,view_state->WHITE_KEYS*view_state->getWhiteSize().y));
        }
        ImGui::EndChild();
    }

private:
    const int octaves = 10;

    void renderSteps(const TimelineContext& ctx) {
        DrawNoteGuides(ctx);
        DrawBars(ctx);
        DrawOctaveLines(ctx);
        renderPattern(ctx);
        renderPlaceHolderNotes(ctx);

        DrawToolEffects(ctx);
        DrawPlayLine(ctx);
    }

    void renderPattern(const TimelineContext& ctx) const;
    void DrawToolEffects(const TimelineContext& ctx);
    void renderPlaceHolderNotes(const TimelineContext& ctx);

    void DrawPlayLine(const TimelineContext& ctx) const {

        float xPos = ctx.cursorPos.x + view_state->getPixelPerBeat(pianoRoll)* (PlayBackManager::instance().getPianoRollPlayheadTicks()/TimeData::instance().PPQ);

        ctx.drawList->AddLine(ImVec2(xPos,ctx.cursorPos.y+ 0),
        ImVec2(xPos,ctx.cursorPos.y+ ctx.height),
        Theme::currentThemeColours.accentPacked, 5
            );
    }

    bool checkIfBarStart(const int beat) const {return beat % (TimeData::instance().timeSignature.getNumerator() * view_state->getRenderedSubDivisions()) == 0;}

    bool checkIfSubDivision(int beat) const
    {
        bool value = true;
        if (beat % view_state->getRenderedSubDivisions() == 0) {
            value = false;
        }
        return value;
    }
     void determineRenderedSubDivisions(const TimelineContext& ctx)
    {
        if (view_state->getSnappedSubDivisions()  < 12)
        {
            view_state->setRenderedSubDivisions(1);
        }
    }

    void DrawBars(auto& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
        increment = increment*2;
        getCurrentBarCount(ctx);
        for (int i = ctx.firstVisibleSubBeat; i <= ctx.lastVisibleSubBeat; i++) {
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * view_state->getPixelPerBeat(zoomFactor::pianoRoll)/view_state->getRenderedSubDivisions(),
                 ctx.cursorPos.y
            );
            ImVec2 beatPosEnd = ImVec2(beatPosStart.x, ctx.cursorPos.y + ctx.height);

            ImU32 colour = Theme::currentThemeColours.beatColourPacked;
            if (checkIfBarStart(i)){colour=Theme::currentThemeColours.barColourPacked;}
            if (checkIfSubDivision(i)){colour=Theme::currentThemeColours.subBeatColourPacked;}

            DrawBarLine(ctx, beatPosStart, beatPosEnd, colour);
        }
    }

    void getCurrentBarCount(const TimelineContext& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleSubBeat*view_state->getRenderedSubDivisions() % increment == 0) {
            bg_tone = !bg_tone;
        }
    }

    static void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, ImU32 colour) {
        ctx.drawList->AddLine(
            start, end, colour,1.0f
        );
    }

    void DrawOctaveLines(const TimelineContext& ctx) const
    {
        float octaveHeight = ctx.noteHeight*12;
        for (unsigned int i = 0; i < octaves; i++) {
            float yPos = ctx.cursorPos.y + i * octaveHeight + ( ctx.noteHeight*8);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.barColourPacked,
                1.0f
            );
        }
    }

    void DrawNoteGuides(const TimelineContext& ctx) const
    {
        int notes = 128;
        bool whiteNote = true;
        int octaveNoteIndex = 0;
        int noteOffset = 5;

        for (auto i{0u}; i < notes; i++) {
            float yPos;
            yPos = (ctx.cursorPos.y + (i) * ctx.noteHeight) ;
            ctx.drawList->AddRectFilled(ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.cursorPos.x+ ctx.width, yPos + ctx.noteHeight ),
                whiteNote ? Theme::currentThemeColours.backgroundAltPacked : Theme::currentThemeColours.backgroundPacked);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.subBeatColourPacked,
                1.0f
            );
            if (((octaveNoteIndex+noteOffset)% 12 < 7 || (octaveNoteIndex+noteOffset)%12 > 7) && (octaveNoteIndex+noteOffset)% 12 != 0 ) {
                whiteNote= !whiteNote;
            }
            octaveNoteIndex++;
        }
    }
};
