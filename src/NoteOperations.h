#pragma once
#include <cstdint>
#include <variant>
#include <vector>
#include "GridStructs.h"

struct SingleNoteCommit{
    uint32_t ID;
    NoteCoordinate coord;
};
struct NotesCommit{NoteMoveDelta delta;};
struct ScaleCommit{float scale;};

using CommitData = std::variant<SingleNoteCommit, NotesCommit, ScaleCommit>;

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
private:
    uint32_t firstNoteAbsolute{0};
    uint32_t endNoteAbsolute{0};
    uint32_t gapFromInitialScalePoint{0};
public:

    float scale= 1.0f;
    void init(uint32_t start,uint32_t end,uint32_t gap) {
        firstNoteAbsolute=start;
        endNoteAbsolute=end;
        gapFromInitialScalePoint=gap;
    }
    void update(NoteCoordinate snapped) override {
        if (snapped.absoluteTime <= 0){return;}
        scale = static_cast<float>(snapped.absoluteTime  - firstNoteAbsolute )/
            static_cast<float>(endNoteAbsolute  - firstNoteAbsolute + gapFromInitialScalePoint);
        std::cout<<"scale: "<<scale<<std::endl;
        if (scale <= 0) {
            scale = 1.0f;
        }
    }
    CommitData commit() override {
        CommitData result;
        result = ScaleCommit(scale);
        clearNotes();
        return result;
    }
};

inline MoveOperation moveOperation;
inline StretchOperation stretchOperation;
inline ScaleOperation scaleOperation;