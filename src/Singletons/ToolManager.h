#pragma once

#include <imgui.h>
#include "../NoteStucts.h"

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

struct MovingNoteSnapshot
{
    uint32_t ID;
    uint32_t absoluteTime;
    uint32_t endAbsoluteTime;
    uint32_t duration;
    uint8_t pitch;

};

//struct noteCoordinate {};

struct MoveOperation
{
    std::vector<MovingNoteSnapshot> movingNotes;

    noteCoordinate originalInputCoordinate;
    short newDeltaPitch;
    int newDeltaTime;
    bool isMovingNote{false};

    void addNotes(MovingNoteSnapshot notes){
        movingNotes.push_back(notes);
        isMovingNote = true;
    }
    void addNotes(std::vector<MovingNoteSnapshot> notes){
        movingNotes.insert(movingNotes.begin(), notes.begin(), notes.end());
        isMovingNote = true;
    }

    void updateMovingNotesPosition(const uint8_t pitch, const uint32_t time)
    {
        newDeltaPitch = pitch - originalInputCoordinate.pitch ;
        newDeltaTime = static_cast<signed>(time - originalInputCoordinate.absoluteTime);

    }

    void commitMovingNotesPosition()
    {
        for (auto& notes : movingNotes)
        {
            notes.absoluteTime += newDeltaPitch;
            notes.endAbsoluteTime  += newDeltaPitch;
            notes.pitch += newDeltaPitch;
        }
        clearNotes();
    }
    void clearNotes() { movingNotes.clear(); isMovingNote = false; }

};

inline MoveOperation moveOperation;

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


private:
    ToolTypes activeNoteTool{EDIT};
    ToolTypes activeArrangerTool{EDIT};
    ToolManager() = default;

    bool boxSelectingActive = false;

    bool isMovingNote = false;


    SelectionCoords selectCoords;
};