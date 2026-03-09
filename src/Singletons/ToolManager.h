#pragma once

#include <imgui.h>

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

struct movingNoteDetails
{
    uint32_t& ID;
    uint32_t absoluteTime;
    uint32_t pitch;
    uint32_t duration;
    uint8_t newPitch;
    uint32_t newAbsoluteTime;
    //ImVec2 noteSize;
    movingNoteDetails(uint32_t& t_ID,uint8_t t_pitch,uint32_t t_duration,uint32_t t_absoluteTime) : ID(t_ID)
    {
        absoluteTime = t_absoluteTime;
        duration = t_duration;
        pitch = t_pitch;
        newPitch = pitch;
        newAbsoluteTime = absoluteTime;
    }
    ;
};

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

    void setMovingNotes(std::vector<movingNoteDetails> t_movingNoteDs)
    {
        for (auto t_movingNoteD : t_movingNoteDs)
        {
            movingNotes.push_back(t_movingNoteD);
        }
    }

    void updateMovingNotesPosition(const uint8_t pitch, const uint32_t time)
    {
        for (auto notes : movingNotes)
        {
            notes.newAbsoluteTime = time;
            notes.newPitch = pitch;
        }

    }

    void setIsMovingNotes(bool state){isMovingNote = state;};

    [[nodiscard]] bool IsMovingNotes() const {return isMovingNote;};

    [[nodiscard]] const std::vector<movingNoteDetails>& getMovingNoteDetails() const {return movingNotes;}

private:
    ToolTypes activeNoteTool{EDIT};
    ToolTypes activeArrangerTool{EDIT};
    ToolManager() = default;


    bool boxSelectingActive = false;

    bool isMovingNote = false;

    std::vector<movingNoteDetails> movingNotes;


    SelectionCoords selectCoords;
};