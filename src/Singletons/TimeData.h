#pragma once
#include <cstdint>
#include <iostream>
#include <ostream>


class TimeData {
public:
    static TimeData& instance() {
        static TimeData instance;
        return instance;
    }

    class TimeSignature {
    public:
        TimeSignature(uint8_t n, uint8_t d) : numerator(n), denominator(d) {}
        void change(uint8_t n, uint8_t d) {
            numerator = n;
            denominator = d;
        }
        [[nodiscard]] uint8_t getNumerator() const { return numerator; }
        [[nodiscard]] uint8_t getDenominator() const { return denominator; }
    private:
        uint8_t numerator;
        uint8_t denominator;
    };
    [[nodiscard]] double getBPM() const { return BPM;}

    static constexpr int PPQ = 960;
    void setBPM(int newBPM) {BPM = newBPM;}
    TimeSignature timeSignature{4, 4};

    [[nodiscard]] double msPerTick() const {
        return 60000.0 / (BPM * static_cast<double>(PPQ));
    }

    double BPM{120};
private:
    TimeData() = default;
};
