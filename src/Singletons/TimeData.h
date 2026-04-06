#pragma once
#include <cstdint>

class TimeData {
public:
    static TimeData& instance() {
        static TimeData instance;
        return instance;
    }

    class TimeSignature {
    public:
        TimeSignature(uint n, uint d) : numerator(n), denominator(d) {}
        void change(uint n, uint d) {
            numerator = n;
            denominator = d;
        }
        uint getNumerator() const { return numerator; }
        uint getDenominator() const { return denominator; }
    private:
        uint numerator;
        uint denominator;
    };
    [[nodiscard]] double getBPM() const { return BPM;}

    static constexpr int PPQ = 960;
    void setBPM(int newBPM) {BPM = newBPM;}
    TimeSignature timeSignature{4, 4};
private:
    double BPM = 120;
    TimeData() = default;
};
