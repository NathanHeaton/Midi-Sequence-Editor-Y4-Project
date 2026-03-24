
//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/ViewState.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"
#include "../../../Singletons/PlayBackManager.h"

class PianoRollComponent
{
public:

    PianoRollComponent() = default;
    ViewState* view_state = &ViewState::instance();
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
            lengthX =  view_state->getPixelPerBar(zoomFactor::pianoRoll) * pattern.m_bars;
            ImGui::Dummy(ImVec2(lengthX ,view_state->WHITE_KEYS*view_state->getWhiteSize().y));
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
        float initialGap;
        float relativeX;
        float relativeY;
        ToolTypes activeTool;

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =ViewState::instance().getWhiteSize().y * ViewState::instance().WHITE_KEYS;
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            scrollY = ImGui::GetScrollY();
            barWidth = 2 * TimeData::instance().timeSignature.getNumerator() * ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll);
            noteHeight = ViewState::instance().getNoteHeight();
            initialGap = (ViewState::instance().getWhiteSize().y * 5.0f)/8.0f;
            auto& view = ViewState::instance();
            firstVisibleSubBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX /
                    (view.getPixelPerBeat(pianoRoll)/ view.getRenderedSubDivisions())) : 0;
            lastVisibleSubBeat = static_cast<int>((scrollX + width)
                / (view.getPixelPerBeat(pianoRoll)/view.getRenderedSubDivisions()));
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
        renderPlaceHolderNotes(ctx);

        DrawToolEffects(ctx);
        DrawPlayHead(ctx);
    }

    void sendNewNote(NoteCoordinate snappedCoordinate);
    void removeNote(const TimelineContext& ctx,uint8_t pitch, uint32_t absoluteTime);
    void moveNote(NoteCoordinate snappedCoordinate);
    void stretchNote(NoteCoordinate snappedCoordinate);
    void scaleNote(NoteCoordinate snappedCoordinate);
    void updateMoveOperation(NoteCoordinate snapped);
    void updateScaleOperation(NoteCoordinate snapped);
    void updateStretchOperation(NoteCoordinate snapped);

    bool isScaleHandleHover(NoteCoordinate hover);

    void edit(const TimelineContext& ctx);
    void HandleMouseInput(const TimelineContext& ctx);
    void renderPattern(const TimelineContext& ctx) const;
    void DrawToolEffects(const TimelineContext& ctx);
    void HandleKeyboardInput(const TimelineContext& ctx);
    NoteCoordinate resolveHoverCoordinate(const TimelineContext& ctx) const;
    NoteCoordinate resolveSnappedCoordinate(const TimelineContext& ctx) const;
    std::vector<NoteSnapshot> setupSnapshots(NoteCoordinate snappedCoordinate);
    void renderPlaceHolderNotes(const TimelineContext& ctx);
    NoteSnapshot createNoteSnapShot(NoteCoordinate snappedCoordinate);
    NoteSnapshot createNoteSnapShotBasedOnID(uint32_t onIds);

    void DrawPlayHead(const TimelineContext& ctx) const
    {

        float xPos = ctx.cursorPos.x + view_state->getPixelPerBeat(pianoRoll)* (PlayBackManager::instance().getPlayheadPositionTicks()/TimeData::instance().PPQ);

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
        float octaveHeight = view_state->getWhiteSize().y *7;
        for (unsigned int i = 0; i < octaves; i++) {
            float yPos = ctx.cursorPos.y + i * octaveHeight + ( view_state->getWhiteSize().y*5);

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
        float noteGap = (view_state->getWhiteSize().y *7.0f)/12.0f;
        int notes = 128;
        bool whiteNote = true;
        int octaveNoteIndex = 0;
        int noteOffset = 5;

        for (auto i{0u}; i < notes; i++) {
            float yPos;
            if (i < 8) {
                yPos = ctx.cursorPos.y + i * ctx.initialGap;
            }
            else {
                yPos = (ctx.cursorPos.y + (i-8) * noteGap) + ctx.initialGap *8;
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
