//
// Created by nathan on 04/11/2025.
//

#ifndef MYPROJECT_MIDIBYTEREADER_H
#define MYPROJECT_MIDIBYTEREADER_H
#include <cstdint>
#include <vector>


class MidiByteReader {

    public:
    MidiByteReader(const std::vector<uint8_t>& m_bytes);
    ~MidiByteReader();
    uint8_t readNext();
    uint8_t peakNext();
    std::vector<uint8_t> readNextN(unsigned long n);
    unsigned long position(){return index;}
    unsigned long remainingBytes();
    bool inRange(int n);
    unsigned short readVariableLength();

    const std::vector<uint8_t>& m_bytes;
    unsigned long index;

};


#endif //MYPROJECT_MIDIBYTEREADER_H