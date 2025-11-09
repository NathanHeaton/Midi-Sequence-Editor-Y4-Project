//
// Created by nathan on 04/11/2025.
//

#include "MidiByteReader.h"




MidiByteReader::MidiByteReader(const std::vector<uint8_t>& bytes) : m_bytes(bytes), index(0) {

}

MidiByteReader::~MidiByteReader() {
}

uint8_t MidiByteReader::readNext() {
    if (!inRange(index + 1)) {
        throw std::out_of_range("Invalid slice range from readNext");
    }
    uint8_t byte = m_bytes[index];
    index++;
    return byte;
}

uint8_t MidiByteReader::peak() {
    return m_bytes[index];
}

std::vector<uint8_t> MidiByteReader::readNextN(unsigned long n) {
    if (inRange(index + n)) {
        int start_point = index;
        index += n;
        return std::vector<uint8_t>(m_bytes.begin() + start_point, m_bytes.begin() + index);
    }
    else {
        throw std::out_of_range("Invalid slice range from readNextN");
    }

}

unsigned long MidiByteReader::remainingBytes() {
    return m_bytes.size() - index;
}

bool MidiByteReader::inRange(int n) {
    if (n < m_bytes.size()) {
        return true;
    }
    return false ;

}

uint32_t MidiByteReader::readVariableLength() {
    uint32_t totalDelta = 0;
    uint8_t byte;

    do {
        byte = readNext();
        totalDelta = (totalDelta << 7) | (byte & 0x7F);
    }
    while (byte & 0x80);

    return totalDelta;
}
