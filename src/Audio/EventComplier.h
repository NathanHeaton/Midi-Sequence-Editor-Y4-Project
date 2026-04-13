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

    static juce::MidiMessage validNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
        pitch    = std::clamp<uint8_t>(pitch,    0,   127);
        velocity = std::clamp<uint8_t>(velocity, 0,   127);
        channel  = std::clamp<uint8_t>(channel,  1,    16);
        auto msg = juce::MidiMessage::noteOn(channel, pitch, velocity);
        return msg;
    }

    static juce::MidiMessage validNoteOff(uint8_t channel, uint8_t pitch) {
        pitch   = std::clamp<uint8_t>(pitch,   0,  127);
        channel = std::clamp<uint8_t>(channel, 1,   16);
        auto msg = juce::MidiMessage::noteOff(channel, pitch);
        return msg;
    }

    static std::vector<ScheduledEvent> compilePattern(const Pattern& pattern, double bpm) {
        std::vector<ScheduledEvent> out;
        const double mpt = msPerTick(bpm);

        for (const auto& pair : pattern.m_noteEvents) {
            const auto* on  = pattern.getMidiEventByID_ptr(pair.onID);
            const auto* off = pattern.getMidiEventByID_ptr(pair.offID);
            if (!on || !off) continue;

            out.push_back({ on->getAbsoluteTime()  * mpt,
                validNoteOn(on->getChannel()+1,  on->getPitch(),  on->getVelocity()) });
            out.push_back({ off->getAbsoluteTime() * mpt,
                validNoteOff(off->getChannel()+1, off->getPitch()) });
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
                    validNoteOn(clip.trackIndex+1,  on->getPitch(),  on->getVelocity()) });
                out.push_back({ offAbs * mpt,
                    validNoteOff(clip.trackIndex+1, off->getPitch()) });
            }
        }
        std::sort(out.begin(), out.end());
        return out;
    }

    static double msPerTick(double bpm) {
        return 60000.0 / (bpm * static_cast<double>(TimeData::PPQ));
    }
};