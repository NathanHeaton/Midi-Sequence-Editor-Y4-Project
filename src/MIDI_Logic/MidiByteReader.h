//
// Created by nathan on 04/11/2025.
//

#ifndef MYPROJECT_MIDIBYTEREADER_H
#define MYPROJECT_MIDIBYTEREADER_H
#include <cstdint>
#include <vector>


class MidiByteReader {

    public:
    MidiByteReader();

    uint8_t readNext();
    uint8_t peakNext();
    std::vector<uint8_t> readNextN();

};


#endif //MYPROJECT_MIDIBYTEREADER_H