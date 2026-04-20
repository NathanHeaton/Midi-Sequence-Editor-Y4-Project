#pragma once
#include <cstdint>

static double BPM{120};

class TimeData {
public:
    static TimeData& instance() {
        static TimeData instance;
        return instance;
    }

    class TimeSignature {
    public:
        TimeSignature(uint32_t n, uint32_t d) : numerator(n), denominator(d) {}
        void change(uint32_t n, uint32_t d) {
            numerator = n;
            denominator = d;
        }
        uint32_t getNumerator() const { return numerator; }
        uint32_t getDenominator() const { return denominator; }
    private:
        uint32_t numerator;
        uint32_t denominator;
    };
    [[nodiscard]] double getBPM() const { return BPM;}

    static constexpr int PPQ = 960;
    void setBPM(int newBPM) {BPM = newBPM;}
    TimeSignature timeSignature{4, 4};

    static double msPerTick() {
        return 60000.0 / (BPM * static_cast<double>(PPQ));
    }

private:
    TimeData() = default;
};
