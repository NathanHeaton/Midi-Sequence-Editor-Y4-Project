#pragma once
#include <vector>
#include <algorithm>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../Singletons/PatternManager.h"
#include "../Singletons/ArrangerManager.h"
#include "../Singletons/TimeData.h"

struct ScheduledEvent {
    double absoluteTimeMs{0.0};
    juce::MidiMessage message;
    bool operator<(const ScheduledEvent& o) const { return absoluteTimeMs < o.absoluteTimeMs; }
};

class EventCompiler {
public:
    // Piano-roll mode: compile one pattern, no clip offset
    static std::vector<ScheduledEvent> compilePattern(const Pattern& pattern, double bpm) {
        std::vector<ScheduledEvent> out;
        const double mpt = msPerTick(bpm);

        for (const auto& pair : pattern.m_noteEvents) {
            const auto* on  = pattern.getMidiEventByID_ptr(pair.onID);
            const auto* off = pattern.getMidiEventByID_ptr(pair.offID);
            if (!on || !off) continue;

            out.push_back({ on->getAbsoluteTime()  * mpt,
                juce::MidiMessage::noteOn (on->getChannel(),  on->getPitch(),  on->getVelocity()) });
            out.push_back({ off->getAbsoluteTime() * mpt,
                juce::MidiMessage::noteOff(off->getChannel(), off->getPitch()) });
        }
        std::sort(out.begin(), out.end());
        return out;
    }

    // Arranger mode: all clips, all tracks, offset by clip.startTime
    static std::vector<ScheduledEvent> compileArranger(double bpm) {
        std::vector<ScheduledEvent> out;
        const double mpt   = msPerTick(bpm);
        const auto*  clips = ArrangerManager::instance().getPatternClips();

        for (const auto& clip : *clips) {
            if (!clip.enabled) continue;
            const Pattern* pat = PatternManager::instance().getPatternByID(clip.patternID);
            if (!pat) continue;

            for (const auto& pair : pat->m_noteEvents) {
                const auto* on  = pat->getMidiEventByID_ptr(pair.onID);
                const auto* off = pat->getMidiEventByID_ptr(pair.offID);
                if (!on || !off) continue;

                uint32_t onAbs  = on->getAbsoluteTime()  + clip.startTime;
                uint32_t offAbs = off->getAbsoluteTime() + clip.startTime;
                if (onAbs >= clip.endTime) continue;           // note starts after clip — skip
                offAbs = std::min(offAbs, clip.endTime);       // clamp note-off to clip boundary

                out.push_back({ onAbs  * mpt,
                    juce::MidiMessage::noteOn (on->getChannel(),  on->getPitch(),  on->getVelocity()) });
                out.push_back({ offAbs * mpt,
                    juce::MidiMessage::noteOff(off->getChannel(), off->getPitch()) });
            }
        }
        std::sort(out.begin(), out.end());
        return out;
    }

    static double msPerTick(double bpm) {
        return 60000.0 / (bpm * static_cast<double>(TimeData::PPQ));
    }
};