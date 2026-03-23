#pragma once
#include <cstdint>
#include <variant>
#include <vector>
#include "NoteStructs.h"

struct SingleNoteCommit{
    uint32_t ID;
    NoteCoordinate coord;
};
struct NotesCommit{NoteMoveDelta delta;};

using CommitData = std::variant<SingleNoteCommit, NotesCommit>;

struct NoteOperation {
    std::vector<NoteSnapshot> notes;
    bool isActive{false};

    void addNotes(NoteSnapshot note) {
        notes.push_back(note);
        isActive = true;
    }
    void addNotes(std::vector<NoteSnapshot> newNotes) {
        notes.insert(notes.begin(), newNotes.begin(), newNotes.end());
        isActive = true;
    }
    void clearNotes() {
        notes.clear();
        isActive = false;
    }

    virtual CommitData commit() = 0;
    virtual void update(NoteCoordinate snapped) = 0;
    virtual ~NoteOperation() = default;
};

struct MoveOperation : NoteOperation{
    NoteCoordinate originalInputCoordinate;
    int8_t newDeltaPitch;
    int newDeltaTime;

    void update(NoteCoordinate snapped) override    {
        newDeltaPitch = snapped.pitch - originalInputCoordinate.pitch ;
        newDeltaTime = static_cast<signed>(snapped.absoluteTime - originalInputCoordinate.absoluteTime);
    }

    [[nodiscard]] CommitData commit() override{
        CommitData result;
        if (notes.size() == 1 ){
            auto note = notes.at(0);
            NoteCoordinate newPos(note.pitch +newDeltaPitch, note.absoluteTime+newDeltaTime);
            result = SingleNoteCommit(note.ID,newPos);
        }
        else {
            NoteMoveDelta newPos(newDeltaPitch, newDeltaTime);
            result = NotesCommit(newPos);
        }
        clearNotes();
        return result;
    }
};

struct StretchOperation : NoteOperation {
private:
    uint32_t originalStartTime{0};
    uint32_t originalEndTime{0};
public:
    int newEndDelta{0};
    void initStretchingNotes(uint32_t startTime,uint32_t endTime) {
        originalStartTime = startTime; originalEndTime = endTime;
    }
    void update(NoteCoordinate snapped) override {
        if (snapped.absoluteTime > originalStartTime) {
            newEndDelta = static_cast<signed>(snapped.absoluteTime - originalEndTime);
        }
    }

    CommitData commit() override{
        CommitData result;
        if (notes.size() == 1 ){
            auto note = notes.at(0);
            NoteCoordinate newPos(0, newEndDelta);
            result = SingleNoteCommit(note.ID,newPos);
        }
        else {
            NoteMoveDelta newPos(0, newEndDelta);
            result = NotesCommit(newPos);
        }
        clearNotes();
        return result;
    }
};


struct ScaleOperation : NoteOperation {
    float scale;
    int firstNoteDelta;
    void update(NoteCoordinate snapped) override
    {
        //
        // scale
    }
    [[maybe_unused]] CommitData commit() override;
};

inline MoveOperation moveOperation;
inline StretchOperation stretchOperation;
//inline ScaleOperation scaleOperation;