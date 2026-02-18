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
        selectCoords.selectP1 = selectP2;
        boxSelectingActive = false;
        normaliseSelectionCoords();
    }

    void normaliseSelectionCoords() {
        SelectionCoords temp;
        temp.selectP1 = {std::min(selectCoords.selectP1.x,selectCoords.selectP2.x),std::min(selectCoords.selectP1.y,selectCoords.selectP2.y)};
        temp.selectP2 = {std::max(selectCoords.selectP1.x,selectCoords.selectP2.x),std::max(selectCoords.selectP1.y,selectCoords.selectP2.y)};
        selectCoords = temp;
    }

private:
    ToolTypes activeNoteTool{EDIT};
    ToolTypes activeArrangerTool{EDIT};
    ToolManager() = default;


    bool boxSelectingActive = false;

    SelectionCoords selectCoords;
};