//
// Created by nathan on 04/01/2026.
//

#ifndef MYPROJECT_PATTERN_H
#define MYPROJECT_PATTERN_H

#include <stdlib.h>
#include <string>
#include "../MIDI_Logic/MIDI_Events/MidiEvent.h"

class Pattern {
    public:


    struct NoteEventPair {
        size_t onIndex;
        size_t offIndex;
    };


    float m_barLength;


    std::string m_title;

    std::vector<MidiEvent> m_events;
    std::vector<NoteEventPair> m_noteEvents;
    int ticksPerQuarterNote;
    int m_bars{8};

    Pattern(std::string t_title, auto barLength) {
        m_barLength = barLength;
        m_title = t_title;
    }
    // from Midi import
    Pattern(std::string t_title, std::vector<MidiEvent>& events, int ticks, auto barLength) : m_events(events) {
        m_title = t_title;
        ticksPerQuarterNote = ticks;
        createNoteEventPairs();
        setLastBar();
    }



    void createNoteEventPairs() {

        struct PendingNoteEvent {
            uint8_t pitch;
            uint8_t channel;
            size_t onIndex;
            PendingNoteEvent(size_t t_onIndex,uint8_t t_pitch, uint8_t t_chan) {
                onIndex = t_onIndex;
                pitch = t_pitch;
                channel = t_chan;
            }
        };
        std::vector<PendingNoteEvent> pendingEvents;
        int cumulativeTime=0;

        for (size_t i=0; i<m_events.size(); i++) {
            cumulativeTime += m_events.at(i).getDelta();
            m_events.at(i).m_absoluteTime = cumulativeTime;

            if (m_events.at(i).isNoteOff() || (m_events.at(i).getVelocity() == 0 && m_events.at(i).isNoteOn())) {
                for (auto it{pendingEvents.begin()};it< pendingEvents.end();it++) {
                    if (m_events.at(i).getPitch() == it->pitch &&
                        m_events.at(i).getChannel() == it->channel) {
                        m_noteEvents.emplace_back(it->onIndex, i);
                        pendingEvents.erase(it);
                        break;
                        }
                }
            }
            else if (m_events.at(i).isNoteOn()) {
                pendingEvents.emplace_back(i,m_events.at(i).getPitch(),m_events.at(i).getChannel());
            }

        }
    }

    // TODO: create search algo to find highest delta in file
    void setLastBar();

    void addNote(MidiEvent noteOn, MidiEvent noteOff) {
        m_events.emplace_back(noteOn);
        m_events.emplace_back(noteOff);
        m_noteEvents.emplace_back(m_events.size() - 1,m_events.size());

    }

    void addNoteSelection(std::vector<MidiEvent> t_events) {
        m_events.insert(m_events.end(), t_events.begin(), t_events.end());

    }

};
#endif //MYPROJECT_PATTERN_H