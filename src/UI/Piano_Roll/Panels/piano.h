#pragma once

#include "../../../Theme.h"
#include <imgui.h>
#include <iostream>
#include "../../../Singletons/ViewState.h"

class Piano
{
public:

    Piano() = default;

    struct pianoVars {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollY;

        pianoVars() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height = ImGui::GetWindowHeight();
            scrollY = ImGui::GetScrollX();
        }
    };
    void create(const float &pianoRollScrollY, float height) {
        whiteKeys = ViewState::instance().WHITE_KEYS;
        blackKeys = ViewState::instance().BLACK_KEYS;
        WHITE_SIZE = ViewState::instance().getWhiteSize();
        BLACK_SIZE = ViewState::instance().getBlackSize();
        blackGap = ViewState::instance().getBlackGap();
        if ( ImGui::BeginChild("piano",ImVec2(WHITE_SIZE.x,height),ImGuiChildFlags_None,
            ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)) {

            pianoVars vars;
            drawWhiteKey(vars);

            ImGui::SetScrollY(pianoRollScrollY);

            drawBlackKey(vars);
            ImGui::Dummy(ImVec2(WHITE_SIZE.x,WHITE_SIZE.y*whiteKeys));
        }
        ImGui::EndChild();
    }

    void drawWhiteKey(pianoVars &vars) const
    {
        int octave = 9;
        float ySize = InitialWhiteNoteGap;
        float runningSize=0;
        for ( int i{0u}; i < whiteKeys; ++i ) {
            if (i == 5) {ySize = WHITE_SIZE.y;}
            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + runningSize);
            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + WHITE_SIZE.x, vars.cursorPos.y + (runningSize + ySize) -1);
            ImVec2 textPos = ImVec2(rectStart.x + ySize-20,rectStart.y+10);
            vars.drawList->AddRectFilled(
                rectStart, rectEnd,
                Theme::cTheme.barColourPacked,
                6.0f
            );
            runningSize += ySize;
            if ((i - 4) % 7 == 0 && i >= 4) {
                char octaveStr[3];
                octaveStr[0] = 'c';
                octaveStr[1] = '0' + octave;
                octaveStr[2] = '\0';
                octave--;
                vars.drawList->AddText(
                    textPos,
                    Theme::cTheme.backgroundPacked,
                    octaveStr
                );
            }
        }
    }
    

    void drawBlackKey(pianoVars &vars) const
    {
        float totalGap = 0;
        int setOf3Count = 3;
        int setOf2Count = 1;
        float ySize = InitialWhiteNoteGap;
        for ( int i{0u}; i < blackKeys; ++i ) {
            if (i == 5) {ySize = WHITE_SIZE.y;}
            if (i == 0) {
                totalGap = totalGap + blackGap;
            }
            else {
                totalGap += ySize;
            }

            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + totalGap);
            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + BLACK_SIZE.x , vars.cursorPos.y + totalGap + BLACK_SIZE.y );

            vars.drawList->AddRectFilled(
            rectStart, rectEnd,
            Theme::cTheme.backgroundPacked,
            6.0f
        );
            if (setOf3Count < 3) {
                setOf3Count++;
            }
            else if (setOf3Count == 3) {
                setOf3Count++;
                totalGap += ySize;
            }
            else if (setOf2Count < 2) {
                setOf2Count++;
            }
            else if (setOf2Count == 2) {
                setOf3Count = 1;
                setOf2Count = 1;
                totalGap += ySize;
            }
        }
    }

private:
    int whiteKeys {};
    int blackKeys {};
    ImVec2 WHITE_SIZE {};
    ImVec2 BLACK_SIZE {};
    float blackGap {};
    const float InitialWhiteNoteGap= (ViewState::instance().getNoteHeight() * 8)/5;
};
