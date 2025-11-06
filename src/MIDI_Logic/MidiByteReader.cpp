//
// Created by nathan on 04/11/2025.
//

#include "MidiByteReader.h"

#include <stdexcept>

#include "juce_core/text/juce_String.h"


MidiByteReader::MidiByteReader(const std::vector<uint8_t>& bytes) : m_bytes(bytes), index(0) {

}

MidiByteReader::~MidiByteReader() {
}

uint8_t MidiByteReader::readNext() {
    auto byte = m_bytes[index];
    if (inRange(index + 1)) {
        index++;
    }
    else {
        throw std::out_of_range("Invalid slice range");
    }

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
        throw std::out_of_range("Invalid slice range");
    }

}

unsigned long MidiByteReader::remainingBytes() {
    return m_bytes.size() - index;
}

bool MidiByteReader::inRange(int n) {
    if (n < m_bytes.size()) {
        return true;
    }
    return false;

}

uint32_t MidiByteReader::readVariableLength() {
    uint32_t totalDelta = 0;
    do{
        totalDelta = totalDelta << 7| (readNext() & 0x7F);
    }
    while ((peak() & 0x80) != 0);


    return totalDelta;
}
