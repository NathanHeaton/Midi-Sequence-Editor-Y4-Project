#pragma once

class TimeData {
public:
    static TimeData& instance() {
        static TimeData instance;
        return instance;
    }

    class TimeSignature {
    public:
        TimeSignature(int n, int d) : numerator(n), denominator(d) {}
        void change(int n, int d) {
            numerator = n;
            denominator = d;
        }
        int getNumerator() const { return numerator; }
        int getDenominator() const { return denominator; }
    private:
        int numerator;
        int denominator;
    };
    [[nodiscard]] double getBPM() const { return BPM;}

    static constexpr int PPQ = 960;
    void setBPM(int newBPM) {BPM = newBPM;}
    TimeSignature timeSignature{4, 4};
private:
    double BPM = 120;
    TimeData() = default;
};
