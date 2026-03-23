#pragma once

#include <imgui.h>
#include "../NoteStructs.h"

enum ToolTypes {
    SELECT,
    EDIT,
    MOVE,
    DELETE,
    SNIP
  };

struct SelectionCoords {
    ImVec2 selectP1;
    ImVec2 selectP2;
};



//struct noteCoordinate {};

struct MoveOperation{
    std::vector<NoteSnapshot> movingNotes;
    NoteCoordinate originalInputCoordinate;
    int8_t newDeltaPitch;
    int newDeltaTime;
    bool isMovingNote{false};

    void addNotes(NoteSnapshot notes){
        movingNotes.push_back(notes);
        isMovingNote = true;
    }
    void addNotes(std::vector<NoteSnapshot> notes){
        movingNotes.insert(movingNotes.begin(), notes.begin(), notes.end());
        isMovingNote = true;
    }

    void updateMovingNotesPosition(const uint8_t pitch, const uint32_t time){
        newDeltaPitch = pitch - originalInputCoordinate.pitch ;
        newDeltaTime = static_cast<signed>(time - originalInputCoordinate.absoluteTime);
    }
    void clearNotes() { movingNotes.clear(); isMovingNote = false; }

};

struct StretchOperation {
private:
    uint32_t originalStartTime{0};
    uint32_t originalEndTime{0};
public:
    std::vector<NoteSnapshot> stretchingNotes;
    int newEndDelta{0};
    bool isStretchingNote{false};

    void initStretchingNotes(uint32_t startTime,uint32_t endTime) {
        originalStartTime = startTime; originalEndTime = endTime;
    }
    void addNotes(NoteSnapshot notes){stretchingNotes.push_back(notes); isStretchingNote = true;}

    void addNotes(std::vector<NoteSnapshot> notes){
        stretchingNotes.insert(stretchingNotes.begin(), notes.begin(), notes.end());
        isStretchingNote = true;
    }
    void updateStretchDelta(uint32_t absolute) {
        if (absolute > originalStartTime) {
            newEndDelta = absolute - originalEndTime;
        }
    }
    void clearNotes() {
        stretchingNotes.clear(); isStretchingNote = false;
    }
};


inline MoveOperation moveOperation;

inline StretchOperation stretchOperation;

class ToolManager {
    public:

    static ToolManager& instance() {
        static ToolManager instance;
        return instance;
    }

    [[nodiscard]] ToolTypes getActiveNoteTool() const { return activeNoteTool; }
    [[nodiscard]] ToolTypes getActiveArrangerTool() const { return activeArrangerTool; }
    [[nodiscard]] bool isBoxSelecting() const { return boxSelectingActive; }
    [[nodiscard]] ImVec2 getSelectionPoint1() const { return selectCoords.selectP1; }
    [[nodiscard]] ImVec2 getSelectionPoint2() const { return selectCoords.selectP2; }
    [[nodiscard]] const SelectionCoords& getSelectionPoints() const { return selectCoords; }


    void setNoteTool(ToolTypes t) { activeNoteTool = t; }
    void setArrangerTool(ToolTypes t) { activeArrangerTool = t; }

    void setSelectionPoint1(ImVec2 selectP1) {
        selectCoords.selectP1 = selectP1;
        boxSelectingActive = true;
    }
    void setSelectionPoint2(ImVec2 selectP2) {
        selectCoords.selectP2 = selectP2;
        boxSelectingActive = false;
        normaliseSelectionCoords();
    }

    void normaliseSelectionCoords() {
        SelectionCoords temp;
        temp.selectP1 = {std::min(selectCoords.selectP1.x,selectCoords.selectP2.x),std::min(selectCoords.selectP1.y,selectCoords.selectP2.y)};
        temp.selectP2 = {std::max(selectCoords.selectP1.x,selectCoords.selectP2.x),std::max(selectCoords.selectP1.y,selectCoords.selectP2.y)};
        selectCoords = temp;
    }

    [[nodiscard]] bool IsMovingNotes() const {return isMovingNote;};

    [[nodiscard]] uint32_t getLastNoteDuration() const {return lastNoteDuration;};
    void setLastNoteDuration(uint32_t newDuration) {lastNoteDuration = newDuration;}

private:
    ToolTypes activeNoteTool{EDIT};
    ToolTypes activeArrangerTool{EDIT};
    ToolManager() = default;

    bool boxSelectingActive = false;

    bool isMovingNote = false;

    uint32_t lastNoteDuration{996};

    SelectionCoords selectCoords;
};