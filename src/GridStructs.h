//
// Created by idad on 13/03/2026.
//

#pragma once

struct NoteCoordinate {
    uint8_t pitch;
    uint32_t absoluteTime;
};

struct NoteSnapshot {
    uint32_t ID;
    uint32_t absoluteTime;
    uint32_t endAbsoluteTime;
    uint32_t duration;
    uint8_t pitch;

};

struct NoteMoveDelta {
    int8_t pitchD;
    int32_t timeDelta;
};

struct ArrangerCoordinate{
    uint32_t time;
    uint32_t track;
};