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

    std::string m_title;

    std::vector<MidiEvent> m_events;
    std::vector<NoteEventPair> m_noteEvents;

    Pattern() {
        m_title = "untitled";
    }

    Pattern(std::string t_title) {
        m_title = t_title;
    }
    // from Midi import
    Pattern(std::string t_title, std::vector<MidiEvent> events) : m_events(events) {
        m_title = t_title;
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

        for (size_t i=0; i<m_events.size(); i++) {
            if (m_events.at(i).isNoteOn()) {
                pendingEvents.emplace_back(i,m_events.at(i).getPitch(),m_events.at(i).getChannel());
            }
            else if (m_events.at(i).isNoteOff()) {
                for (PendingNoteEvent pendingNote : pendingEvents) {
                    if (m_events.at(i).getPitch() == pendingNote.pitch &&
                        m_events.at(i).getChannel() == pendingNote.channel) {
                        m_noteEvents.emplace_back(pendingNote.onIndex, i);
                        pendingEvents.erase(pendingEvents.begin() + i);
                        break;
                    }
                }
            }
        }
    }

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