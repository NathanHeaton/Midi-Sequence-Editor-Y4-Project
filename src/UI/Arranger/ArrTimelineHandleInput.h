#pragma once
#include "../../Singletons/ArrangerManager.h"
#include "../../Singletons/ToolManager.h"
#include "../../Singletons/ViewState.h"
#include "../../NoteOperations.h"
#include "../../utils.h"
#include "ArrangerContext.h"
#include "../../Singletons/PanelManager.h"

class ArrTimelineHandleInput
{
public:

    // Called once per frame from PianoRollComponent::create().
    void process(const ArrangerContext& ctx) {
        handleMouseInput(ctx);
        handleKeyboardInput(ctx);
    }

private:
    // ---------------------------------------------------------------
    // Top-level input routing
    // ---------------------------------------------------------------
    ArrangerCoordinate hover{0,0};
    ArrangerCoordinate snapped{0,0};
    void handleMouseInput(const ArrangerContext& ctx) {
        if (!ImGui::IsWindowHovered()) {
            return;
        }
        hover   = resolveHoverCoordinate(ctx);
        snapped = resolveSnappedCoordinate(ctx);
        switch (ctx.activeTool) {
            // add other fields once structre is established
        // case MOVE:   break;
        // case DELETE: break;
        case EDIT:   handleEditTool(ctx);   break;
        }
    }

    void handleKeyboardInput(const ArrangerContext& ctx)
    {
        if (ImGui::IsKeyDown(ImGuiMod_Shift)) {
            //     if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))  ;
            //     if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))    ;
            //     if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))  ;
            //     if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) ;
        }
        if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
            // if      (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) ;
            // else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))    ;
            // else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))  ;
            // else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) ;
            // else if (ImGui::IsKeyPressed(ImGuiKey_Z))  { /* undo — TODO */ }
            // else if (ImGui::IsKeyPressed(ImGuiKey_A))  { /* select all — TODO */ }
            // else if (ImGui::IsKeyPressed(ImGuiKey_C)) {
            //     PatternManager::instance().copyEventsSelectedEvents();
            // }
            // else if (ImGui::IsKeyPressed(ImGuiKey_V)) {
            //     if (!ImGui::IsWindowHovered()) {// paste in-place
            //     } else {
            //     }
            // }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {

            if      (ImGui::IsKeyPressed(ImGuiKey_E)) ToolManager::instance().setPianoRollTool(ToolTypes::EDIT);
            else if (ImGui::IsKeyPressed(ImGuiKey_S)) ToolManager::instance().setPianoRollTool(ToolTypes::SELECT);
        }
    }
    void handleEditTool(const ArrangerContext& ctx) {
        auto& am = ArrangerManager::instance();
        auto ClipHovered = am.resolveHoverState(hover);
        uint32_t pID;
        if (ClipHovered.hoverState != NoHover) {
            pID = am.getClipByID(ClipHovered.ID)->patternID;
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                std::cout << "opening pattern" << std::endl;
                PanelManager::instance().openPatern(pID);
                return;
            }
        }
        if (am.moveOperation.isActive) {
            updateMoveOperation();
            return;
        }
        if (am.resizeOperation.isActive) {
            updateResizeOperation();
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

            switch (ClipHovered.hoverState) {
                case CenterHover: {
                    am.moveOperation.begin(ClipHovered.ID, snapped, am.getClipByID(ClipHovered.ID)->startTime);
                    PatternManager::instance().setActivePatternID(pID);
                    break;
                }
                case EdgeHover:   {
                    am.resizeOperation.begin(ClipHovered.ID);
                    PatternManager::instance().setActivePatternID(pID);
                    break;
                }
                case NoHover:     {am.addClip(snapped);     break;}
            }
        }
        else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            am.removeClip(hover);
        }
    }

    void updateMoveOperation() {
        auto& am = ArrangerManager::instance();;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            am.moveOperation.update(snapped);

            am.moveClip(am.moveOperation.clipID,
                am.moveOperation.currentCoord);
        }
        else {
            am.moveOperation.reset();
        }
    }

    void updateResizeOperation() {
        auto& am = ArrangerManager::instance();
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            am.resizeClip(am.resizeOperation.clipID,
                snapped.time);
        }
        else {
            am.resizeOperation.reset();
        }
    }

    [[nodiscard]] ArrangerCoordinate resolveHoverCoordinate(const ArrangerContext& ctx) const {
        auto* vs = &ViewState::instance();
        int absoluteTime = static_cast<int>(
            ctx.relativeX / vs->getPixelPerBeat(zoomFactor::arranger) * TimeData::PPQ);

        auto trackAmount = ArrangerManager::instance().getTrackAmount();
        int track =std::clamp(static_cast<int>(ctx.relativeY / (ViewState::instance().getTrackHeight())),0,trackAmount);
        return ArrangerCoordinate(static_cast<uint32_t>(absoluteTime), static_cast<uint32_t>(track));
    }

    [[nodiscard]] ArrangerCoordinate resolveSnappedCoordinate(const ArrangerContext& ctx) const {
        auto* vs = &ViewState::instance();
        int patternTime = static_cast<int>(
                    ctx.relativeX / vs->getPixelPerBeat(arranger) * vs->getSnappedSubDivisionsArr());
        int snappedTime = patternTime * (static_cast<float>(TimeData::PPQ) / vs->getSnappedSubDivisionsArr());
        auto trackAmount = ArrangerManager::instance().getTrackAmount();
        int track =std::clamp(static_cast<int>(ctx.relativeY / (ViewState::instance().getTrackHeight())),0,trackAmount);
        return ArrangerCoordinate(static_cast<uint32_t>(snappedTime), static_cast<uint32_t>(track));
    }
};
