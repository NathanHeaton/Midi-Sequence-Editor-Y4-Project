//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/SessionData.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"

class PianoRollComponent
{
public:
    PianoRollComponent() = default;
    SessionData* s = &SessionData::instance();
    bool bg_tone = false;

    void create(float &scrollY,float &scrollX, float& lengthX) {
        if (ImGui::BeginChild("piano grid",ImVec2(0,0),
            ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)) {
            TimelineContext ctx;

            renderSteps(ctx);
            HandleMouseInput(ctx);
            HandleKeyboardInput(ctx);

            ImGui::SetScrollX(scrollX);
            ImGui::SetScrollY(scrollY);
            auto& pattern = PatternManager::instance().getCurrentPattern();
            lengthX =  SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll) * pattern.m_bars;
            ImGui::Dummy(ImVec2(lengthX ,s->getWhiteKeys()*s->getWhiteSize().y));
        }
        ImGui::EndChild();
    }



private:
    const int octaves = 10;
    struct TimelineContext {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollX;
        float scrollY;
        float width;
        int firstVisibleSubBeat;
        int lastVisibleSubBeat;
        float barWidth;
        float noteHeight;
        float intialGap;
        float relativeX;
        float relativeY;
        ToolTypes activeTool;

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =SessionData::instance().getWhiteSize().y * SessionData::instance().getWhiteKeys();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            scrollY = ImGui::GetScrollY();
            barWidth = 2 * SessionData::instance().timeSignature.getNumerator() * SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll);
            noteHeight = SessionData::instance().getNoteHeight();
            intialGap = (SessionData::instance().getWhiteSize().y * 5.0f)/8.0f;
            auto& session = SessionData::instance();
            firstVisibleSubBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX /
                    (session.getPixelPerBeat(pianoRoll)/ session.getRenderedSubDivisions())) : 0;
            lastVisibleSubBeat = static_cast<int>((scrollX + width)
                / (session.getPixelPerBeat(pianoRoll)/session.getRenderedSubDivisions()));
            ImVec2 mousePos = ImGui::GetMousePos();
            relativeX = mousePos.x - cursorPos.x;
            relativeY = mousePos.y - cursorPos.y;
            activeTool = ToolManager::instance().getActiveNoteTool();
        }
    };

    void renderSteps(const TimelineContext& ctx) {
        DrawNoteGuides(ctx);
        DrawBars(ctx);
        DrawOctaveLines(ctx);
        renderPattern(ctx);
        DrawToolEffects(ctx);
    }

    void HandleMouseInput(const TimelineContext& ctx);
    void renderPattern(const TimelineContext& ctx);
    void DrawToolEffects(const TimelineContext& ctx);
    void HandleKeyboardInput(const TimelineContext& ctx);

    bool checkIfBarStart(int beat) {return beat % (s->timeSignature.getNumerator() * s->getRenderedSubDivisions()) == 0;}

    bool checkIfSubDivision(int beat) {
        bool value = true;
        if (beat % s->timeSignature.getNumerator() == 0) {
            value = false;
        }
        return value;
    }

    void DrawBars(auto& ctx) {
        int increment = s->timeSignature.getNumerator();
        increment = increment*2;
        getCurrentBarCount(ctx);
        for (int i = ctx.firstVisibleSubBeat; i <= ctx.lastVisibleSubBeat; i++) {
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * s->getPixelPerBeat(zoomFactor::pianoRoll)/s->getRenderedSubDivisions(),
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
        int increment = s->timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleSubBeat*s->getRenderedSubDivisions() % increment == 0) {
            bg_tone = !bg_tone;
        }
    }

    void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, ImU32 colour) {
        ctx.drawList->AddLine(
            start, end, colour,1.0f
        );
    }

    void DrawOctaveLines(const TimelineContext& ctx) {
        float octaveHeight = s->getWhiteSize().y *7;
        for (unsigned int i = 0; i < octaves; i++) {
            float yPos = ctx.cursorPos.y + i * octaveHeight + ( s->getWhiteSize().y*5);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.barColourPacked,
                1.0f
            );
        }
    }

    void DrawNoteGuides(const TimelineContext& ctx) {
        float noteGap = (s->getWhiteSize().y *7.0f)/12.0f;
        int notes = 128;
        bool whiteNote = true;
        int octaveNoteIndex = 0;
        int noteOffset = 5;

        for (auto i{0u}; i < notes; i++) {
            float yPos;
            if (i < 8) {
                yPos = ctx.cursorPos.y + i * ctx.intialGap;
            }
            else {
                yPos = (ctx.cursorPos.y + (i-8) * noteGap) + ctx.intialGap *8;
            }
            ctx.drawList->AddRectFilled(ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.cursorPos.x+ ctx.width, yPos + noteGap ),
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
