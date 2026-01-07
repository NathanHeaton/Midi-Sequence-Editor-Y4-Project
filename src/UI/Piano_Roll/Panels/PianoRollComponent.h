//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../SessionData.h"

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

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =SessionData::instance().getWhiteSize().y * SessionData::instance().getWhiteKeys();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            scrollY = ImGui::GetScrollY();
            barWidth = 2 * SessionData::instance().timeSignature.getNumerator() * SessionData::instance().getPixelPerBeatPianoRoll();
            auto& session = SessionData::instance();
            firstVisibleBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX / session.getPixelPerBeatPianoRoll()) : 0;
            lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeatPianoRoll());
        }
    };

    // void HandleMouseInput() {
    //     TimelineContext ctx;
    //
    //     ImVec2 mousePos = ImGui::GetMousePos();
    //     bool isHovered = ImGui::IsWindowHovered();
    //
    //     if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //         float relativeX = mousePos.x - ctx.cursorPos.x + ctx.scrollX;
    //         float relativeY = mousePos.y - ctx.cursorPos.y + ctx.scrollY;
    //
    //         int beat = static_cast<int>(relativeX / s->getPixelPerBeatPianoRoll());
    //         float noteGap = (s->getWhiteSize().y * 7.0f) / 12.0f;
    //         int note = static_cast<int>(relativeY / noteGap);
    //
    //         note = std::clamp(note, 0, 83);
    //
    //         printf("Clicked at beat: %d, note: %d\n", beat, note);
    //         // TODO: Add note to SessionData or wherever you store notes
    //     }
    //
    //     if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    //     }
    // }
    void create(float &pianoRollScrollY) {
        if (ImGui::BeginChild("piano grid",ImVec2(0,0),
            ImGuiChildFlags_None,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {

            DrawNoteGuides();
            DrawBars();
            DrawOctaveLines();
            //HandleMouseInput();
            renderPattern();

            pianoRollScrollY = ImGui::GetScrollY();
            ImGui::Dummy(ImVec2(1000,s->getWhiteKeys()*s->getWhiteSize().y));
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
                ctx.cursorPos.x + i * s->getPixelPerBeatPianoRoll(),
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
            float yPos = ctx.cursorPos.y + i * octaveHeight;

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
        auto& pattern = SessionData::instance().getCurrentPattern();
        auto& noteData = pattern.m_events;
        for (auto noteIndices : pattern.m_noteEvents) {
            float yStart = ctx.cursorPos.y + (noteData.at(noteIndices.onIndex).getPitch()*s->getWhiteSize().y);
            float xStart = ctx.cursorPos.x;

            ctx.drawList->AddRectFilled(
                ImVec2(xStart, yStart),
                ImVec2(xStart + 60,yStart+ s->getWhiteSize().y),
                    Theme::currentThemeColours.barColourPacked);
        }

    }


    void DrawNoteGuides() {
        TimelineContext ctx;
        float noteGap = (s->getWhiteSize().y *7.0f)/12.0f;
        int notes = 128;
        bool whiteNote = true;
        for (auto i{0u}; i < notes; i++) {
            float yPos = ctx.cursorPos.y + i * noteGap;
            if (notes%2 == 0) {
                whiteNote= !whiteNote;
            }
            ctx.drawList->AddRectFilled(ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.cursorPos.x+ ctx.width, yPos+ noteGap),
                whiteNote ? Theme::currentThemeColours.backgroundAltPacked : Theme::currentThemeColours.backgroundPacked);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.beatColourPacked,
                1.0f
            );
        }
    }
};
