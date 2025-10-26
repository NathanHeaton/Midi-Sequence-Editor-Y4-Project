//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_TRACKCHUNK_H
#define MYPROJECT_TRACKCHUNK_H
#include <cstdint>
#include <vector>
#include "Note.h"
#include "../utils.h"

class TrackChunk {
    public:
    TrackChunk(std::vector<uint8_t> bytes);
    ~TrackChunk();
    void printMidiInfo();
    void createNoteEvents();

    std::vector<Note> notes;

    std::vector<uint8_t> m_trackBytes;

    private:
};


#endif //MYPROJECT_TRACKCHUNK_H