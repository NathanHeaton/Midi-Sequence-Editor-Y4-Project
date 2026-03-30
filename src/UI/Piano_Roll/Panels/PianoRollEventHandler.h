#pragma once

#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"
#include "../../../Singletons/ViewState.h"
#include "../../../NoteOperations.h"
#include "../../../utils.h"
#include "TimelineContext.h"

class PianoRollInputHandler {
public:

    // Called once per frame from PianoRollComponent::create().
    void process(const TimelineContext& ctx) {
        handleMouseInput(ctx);
        handleKeyboardInput(ctx);
    }

private:

    // ---------------------------------------------------------------
    // Top-level input routing
    // ---------------------------------------------------------------
    NoteCoordinate hover;
    NoteCoordinate snapped;
    void handleMouseInput(const TimelineContext& ctx) {
        if (!ImGui::IsWindowHovered()) {
            return;
        }
        hover   = resolveHoverCoordinate(ctx);
        snapped = resolveSnappedCoordinate(ctx);
        switch (ctx.activeTool) {
            case MOVE:   break;
            case DELETE: break;
            case SNIP:   break;
            case SELECT: handleSelectTool(ctx); break;
            case EDIT:   handleEditTool(ctx);   break;
        }
    }

    void handleKeyboardInput(const TimelineContext& ctx) {
        if (ImGui::IsKeyDown(ImGuiMod_Shift)) {
            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))  PatternManager::instance().pitchShiftSelection(-1);
            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))    PatternManager::instance().pitchShiftSelection(+1);
            if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))  PatternManager::instance().timeShiftSelection(-100);
            if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) PatternManager::instance().timeShiftSelection(+100);
        }
        if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
            if      (ImGui::IsKeyPressed(ImGuiKey_DownArrow))  PatternManager::instance().pitchShiftSelection(-12);
            else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))    PatternManager::instance().pitchShiftSelection(+12);
            else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))  PatternManager::instance().timeShiftSelection(-120);
            else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) PatternManager::instance().timeShiftSelection(+120);
            else if (ImGui::IsKeyPressed(ImGuiKey_Z))  { /* undo — TODO */ }
            else if (ImGui::IsKeyPressed(ImGuiKey_A))  { /* select all — TODO */ }
            else if (ImGui::IsKeyPressed(ImGuiKey_C)) {
                PatternManager::instance().copyEventsSelectedEvents();
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_V)) {
                if (!ImGui::IsWindowHovered()) {
                    PatternManager::instance().pasteEvents();   // paste in-place
                } else {
                    PatternManager::instance().pasteEventsOnMouse(snapped);
                }
            }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
            if (PatternManager::instance().areNotesSelected())
                PatternManager::instance().deleteSelection();
            else
                PatternManager::instance().clearPattern();
        }
        if      (ImGui::IsKeyPressed(ImGuiKey_E)) ToolManager::instance().setPianoRollTool(ToolTypes::EDIT);
        else if (ImGui::IsKeyPressed(ImGuiKey_S)) ToolManager::instance().setPianoRollTool(ToolTypes::SELECT);
    }

    // ---------------------------------------------------------------
    // Tool handlers
    // ---------------------------------------------------------------

    void handleSelectTool(const TimelineContext& ctx) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ToolManager::instance().setSelectionPoint1(ImVec2{ctx.relativeX, ctx.relativeY});
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            ToolManager::instance().setSelectionPoint2(ImVec2{ctx.relativeX, ctx.relativeY});
            PatternManager::instance().setSelection(ToolManager::instance().getSelectionPoints());
        }
    }

    void handleEditTool(const TimelineContext& ctx) {
        const auto hoverState = PatternManager::instance().getNoteHoverState(hover);

        // Tick any active operation
        if      (moveOperation.isActive)    updateMoveOperation(snapped);
        else if (stretchOperation.isActive) updateStretchOperation(snapped);
        else if (scaleOperation.isActive)   updateScaleOperation(hover);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (isScaleHandleHover(hover, ctx)) {
                beginScaleNote(hover);
            } else {
                switch (hoverState) {
                    case NoteCenterHover: beginMoveNote(snapped);    break;
                    case NoteEdgeHover:   beginStretchNote(snapped); break;
                    default:              sendNewNote(snapped);       break;
                }
            }
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
            ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            PatternManager::instance().removeNoteFromPattern(hover);
        }
    }

    // ---------------------------------------------------------------
    // Operation update / commit
    // ---------------------------------------------------------------

    void updateMoveOperation(NoteCoordinate snapped) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            moveOperation.update(snapped);
        } else {
            auto commit = moveOperation.commit();
            if (std::holds_alternative<SingleNoteCommit>(commit)) {
                auto& c = std::get<SingleNoteCommit>(commit);
                PatternManager::instance().moveNoteEvent(c.ID, c.coord);
            } else {
                auto& c = std::get<NotesCommit>(commit);
                PatternManager::instance().moveSelection(c.delta);
            }
            PatternManager::instance().showAllEvents();
        }
    }

    void updateStretchOperation(NoteCoordinate snapped) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            stretchOperation.update(snapped);
        } else {
            auto commit = stretchOperation.commit();
            if (std::holds_alternative<SingleNoteCommit>(commit)) {
                auto& c = std::get<SingleNoteCommit>(commit);
                PatternManager::instance().stretchNoteEvent(c.ID, c.coord.absoluteTime);
            } else {
                auto& c = std::get<NotesCommit>(commit);
                PatternManager::instance().stretchSelection(c.delta);
            }
            PatternManager::instance().showAllEvents();
        }
    }

    void updateScaleOperation(NoteCoordinate hover) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            scaleOperation.update(hover);
        } else {
            auto commit = scaleOperation.commit();
            if (std::holds_alternative<ScaleCommit>(commit)) {
                PatternManager::instance().scaleSelection(std::get<ScaleCommit>(commit).scale);
            } else {
                std::cout << "error with scale commit" << std::endl;
            }
            PatternManager::instance().showAllEvents();
        }
    }

    // ---------------------------------------------------------------
    // Operation beginners (previously named moveNote / stretchNote / scaleNote)
    // ---------------------------------------------------------------

    void sendNewNote(NoteCoordinate snapped) {
        PatternManager::instance().addNoteToPattern(
            snapped.pitch,
            static_cast<signed>(snapped.absoluteTime),
            ToolManager::instance().getLastNoteDuration());
    }

    void beginMoveNote(NoteCoordinate snapped) {
        auto snapshots = setupSnapshots(snapped);
        moveOperation.originalInputCoordinate = snapped;
        moveOperation.addNotes(snapshots);
    }

    void beginStretchNote(NoteCoordinate snapped) {
        auto snapshots = setupSnapshots(snapped);
        stretchOperation.initStretchingNotes(snapshots[0].absoluteTime, snapshots[0].endAbsoluteTime);
        stretchOperation.addNotes(snapshots);
    }

    void beginScaleNote(NoteCoordinate hover) {
        auto snapshots = setupSnapshots(hover);
        scaleOperation.addNotes(snapshots);
        scaleOperation.init(
            scaleOperation.notes.at(0).absoluteTime,
            scaleOperation.notes.at(0).endAbsoluteTime,
            hover.absoluteTime - scaleOperation.notes.at(0).endAbsoluteTime);
    }

    // ---------------------------------------------------------------
    // Coordinate resolution
    // ---------------------------------------------------------------

    [[nodiscard]] NoteCoordinate resolveHoverCoordinate(const TimelineContext& ctx) const {
        auto* vs = &ViewState::instance();
        int absoluteTime = static_cast<int>(
            ctx.relativeX / vs->getPixelPerBeat(pianoRoll) * TimeData::PPQ);
        int pitch = std::clamp(127 - static_cast<int>(ctx.relativeY / ctx.noteHeight), 0, 127);
        return NoteCoordinate(pitch, static_cast<uint32_t>(absoluteTime));
    }

    [[nodiscard]] NoteCoordinate resolveSnappedCoordinate(const TimelineContext& ctx) const {
        auto* vs = &ViewState::instance();
        int noteTime = static_cast<int>(
            ctx.relativeX / vs->getPixelPerBeat(pianoRoll) * vs->getSnappedSubDivisions());
        int snappedTime = noteTime * (static_cast<float>(TimeData::PPQ) / vs->getSnappedSubDivisions());
        int pitch = std::clamp(127 - static_cast<int>(ctx.relativeY / ctx.noteHeight), 0, 127);
        return NoteCoordinate(pitch, static_cast<uint32_t>(snappedTime));
    }

    // ---------------------------------------------------------------
    // Scale handle hit-test
    // ---------------------------------------------------------------

    [[nodiscard]] bool isScaleHandleHover(NoteCoordinate hover, const TimelineContext& ctx) const {
        auto* tm = &ToolManager::instance();
        auto p1 = tm->getScaleHandlePosition();
        auto p2 = addImVec2(tm->getScaleHandleSize(), p1);
        auto s1 = ImVec2(
            (static_cast<float>(hover.absoluteTime) / TimeData::PPQ)
                * ViewState::instance().getPixelPerBeat(pianoRoll) + ctx.cursorPos.x,
            (127 - hover.pitch) * ViewState::instance().getNoteHeight() + ctx.cursorPos.y);
        return s1.x > p1.x && s1.x < p2.x
            && s1.y > p1.y && s1.y < p2.y;
    }

    // ---------------------------------------------------------------
    // Snapshot helpers
    // ---------------------------------------------------------------

    std::vector<NoteSnapshot> setupSnapshots(NoteCoordinate coordinate) {
        auto* pattern = &PatternManager::instance().getCurrentPattern();
        std::vector<NoteSnapshot> snapshots;

        if (!pattern->m_selectedNoteOnIDs.empty()) {
            for (uint32_t id : pattern->m_selectedNoteOnIDs) {
                PatternManager::instance().hideNoteEventByID(id);
                snapshots.push_back(snapshotFromID(id));
            }
        } else {
            snapshots.push_back(snapshotFromCoordinate(coordinate));
        }
        return snapshots;
    }

    NoteSnapshot snapshotFromID(uint32_t onID) {
        auto* pattern = &PatternManager::instance().getCurrentPattern();
        auto  pair    = *pattern->getEventIDPairFromOnID(onID);
        auto* on      = pattern->getMidiEventByID_ptr(pair.onID);
        auto* off     = pattern->getMidiEventByID_ptr(pair.offID);
        return NoteSnapshot(pair.onID,
            on->getAbsoluteTime(), off->getAbsoluteTime(),
            off->getAbsoluteTime() - on->getAbsoluteTime(),
            on->getPitch());
    }

    NoteSnapshot snapshotFromCoordinate(NoteCoordinate coordinate) {
        auto  pair    = PatternManager::instance().getNoteEventPairFromCoordinate(coordinate);
        auto* pattern = &PatternManager::instance().getCurrentPattern();
        auto* on      = pattern->getMidiEventByID_ptr(pair.onID);
        auto* off     = pattern->getMidiEventByID_ptr(pair.offID);
        PatternManager::instance().hideNoteEventByID(pair.onID);
        return NoteSnapshot(pair.onID,
            on->getAbsoluteTime(), off->getAbsoluteTime(),
            off->getAbsoluteTime() - on->getAbsoluteTime(),
            on->getPitch());
    }
};