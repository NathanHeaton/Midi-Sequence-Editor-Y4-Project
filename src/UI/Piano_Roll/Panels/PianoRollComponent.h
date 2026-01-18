//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <filesystem>

#include "../../../Theme.h"
#include "../../../Singletons/SessionData.h"

//#include "PianoRoll/PianoKeys.h"

class PianoRollComponent
{
public:

    PianoRollComponent() = default;

    SessionData* s = &SessionData::instance();

    bool bg_tone = false;

    struct TimelineContext {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollX;
        float scrollY;
        float width;
        int firstVisibleBeat;
        int lastVisibleBeat;
        float barWidth;
        float noteHeight;

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =SessionData::instance().getWhiteSize().y * SessionData::instance().getWhiteKeys();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            scrollY = ImGui::GetScrollY();
            barWidth = 2 * SessionData::instance().timeSignature.getNumerator() * SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll);
            noteHeight = SessionData::instance().getWhiteSize().y * 7 /12;
            auto& session = SessionData::instance();
            firstVisibleBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX / session.getPixelPerBeat(zoomFactor::pianoRoll)) : 0;
            lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeat(zoomFactor::pianoRoll));
        }
    };

    void HandleMouseInput() {
        TimelineContext ctx;

        ImVec2 mousePos = ImGui::GetMousePos();
        bool isHovered = ImGui::IsWindowHovered();

        if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            float relativeX = mousePos.x - ctx.cursorPos.x;
            float relativeY = mousePos.y - ctx.cursorPos.y;

            int beat = static_cast<int>(relativeX / s->getPixelPerBeat(zoomFactor::pianoRoll));
            int absoluteTime = beat * s->getPPQ();
            int pitch = static_cast<int>(relativeY / ctx.noteHeight);
            pitch = std::clamp(pitch, 0, 127);
            pitch = 127 - pitch;
            printf("Absolute Time clicked: %d, pitch: %d\n", absoluteTime, pitch);
            int duration = s->getPPQ();

            s->addNoteToPattern(pitch,  absoluteTime,  duration);
        }

        if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        }
    }

    void create(float &scrollY,float &scrollX, float& lengthX) {
        if (ImGui::BeginChild("piano grid",ImVec2(0,0),
            ImGuiChildFlags_None,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
            DrawNoteGuides();
            DrawBars();
            DrawOctaveLines();
            HandleMouseInput();
            renderPattern();


            scrollY = ImGui::GetScrollY();
            scrollX = ImGui::GetScrollX();
            auto& pattern = s->getCurrentPattern();
            lengthX =  SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll) * pattern.m_bars;
            ImGui::Dummy(ImVec2(lengthX ,s->getWhiteKeys()*s->getWhiteSize().y));
        }
        ImGui::EndChild();

    }

private:

    const int octaves = 10;

    bool checkIfBarStart(int beat) {
        return beat % s->timeSignature.getNumerator() == 0;
    }

    void DrawBars() {
        TimelineContext ctx;

        int increment = s->timeSignature.getNumerator();
        increment = increment*2;

        getCurrentBarCount(ctx);
        for (int i = ctx.firstVisibleBeat; i <= ctx.lastVisibleBeat; i++) {
            bool barStart = checkIfBarStart(i);
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * s->getPixelPerBeat(zoomFactor::pianoRoll),
                barStart ? ctx.cursorPos.y : ctx.cursorPos.y
            );
            ImVec2 beatPosEnd = ImVec2(beatPosStart.x, ctx.cursorPos.y + ctx.height);

            DrawBarLine(ctx, beatPosStart, beatPosEnd, barStart);
        }
    }

    void getCurrentBarCount(const TimelineContext& ctx) {
        int increment = s->timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleBeat % increment == 0) {
            bg_tone = !bg_tone;
        }
    }

    void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, bool barStart) {
        ctx.drawList->AddLine(
            start, end,
            barStart ? Theme::currentThemeColours.barColourPacked
                     : Theme::currentThemeColours.beatColourPacked,
            1.0f
        );
    }


    void DrawOctaveLines() {
        TimelineContext ctx;
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

    void renderPattern() {
        TimelineContext ctx;
        auto& pattern = s->getCurrentPattern();
        auto& noteData = pattern.m_events;

        for (auto noteIndices : pattern.m_noteEvents) {
            auto& onIndex = noteData.at(noteIndices.onIndex);
            auto offIndex = noteData.at(noteIndices.offIndex);

            float startDelta =0;
            if (onIndex.m_absoluteTime != 0){startDelta =
                static_cast<float>(onIndex.m_absoluteTime) / s->getPPQ();}

            float endDelta=0;
            if (offIndex.m_absoluteTime != 0){endDelta =
                static_cast<float>(offIndex.m_absoluteTime) / s->getPPQ();}

            float startPixel = s->getPixelPerBeat(zoomFactor::pianoRoll) * startDelta;
            float endPixel = s->getPixelPerBeat(zoomFactor::pianoRoll) * endDelta ;

            float yStart = ctx.cursorPos.y + (ctx.noteHeight*128.0f) - (noteData.at(noteIndices.onIndex).getPitch()*ctx.noteHeight);
            float xStart = ctx.cursorPos.x + startPixel;
            float xEnd =  ctx.cursorPos.x + endPixel;
            float yEnd = yStart + ctx.noteHeight;

            ctx.drawList->AddRectFilled(
                ImVec2(xStart, yStart),
                ImVec2(xEnd,yEnd),
                    Theme::currentThemeColours.barColourPacked);

        }


    }


    void DrawNoteGuides() {
        TimelineContext ctx;
        float noteGap = (s->getWhiteSize().y *7.0f)/12.0f;
        int notes = 128;
        bool whiteNote = true;
        int octaveNoteIndex = 0;
        int noteOffset = 5;

        for (auto i{0u}; i < notes; i++) {
            float yPos = ctx.cursorPos.y + i * noteGap;

            ctx.drawList->AddRectFilled(ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.cursorPos.x+ ctx.width, yPos+ noteGap),
                whiteNote ? Theme::currentThemeColours.backgroundAltPacked : Theme::currentThemeColours.backgroundPacked);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.beatColourPacked,
                1.0f
            );

            if (((octaveNoteIndex+noteOffset)% 12 < 7 || (octaveNoteIndex+noteOffset)%12 > 7) && (octaveNoteIndex+noteOffset)% 12 != 0 ) {
                whiteNote= !whiteNote;
            }

            octaveNoteIndex++;

        }
    }
};
