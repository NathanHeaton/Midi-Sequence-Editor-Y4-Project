#include "Pattern.h"
#include "../Singletons/SessionData.h"
#include <iostream>

//
// Created by nathan on 11/01/2026.
//

void Pattern::setLastBar() {
    const auto barSize = SessionData::instance().getPPQ() * SessionData::instance().timeSignature.getNumerator();
    int endAbsolute = m_events.back().m_absoluteTime;

    float bars = static_cast<float>(endAbsolute) / static_cast<float>(barSize);
    m_bars = ceil(bars);
}

void Pattern::createNoteEventPairs(){
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

    for (size_t i=0; i < m_events.size(); i++) {
        cumulativeTime += m_events.at(i).getDelta();
        m_events.at(i).m_absoluteTime = cumulativeTime;
        if (m_events.at(i).isNoteOff() || (m_events.at(i).getVelocity() == 0 && m_events.at(i).isNoteOn())) {
            for (auto it{pendingEvents.begin()};it< pendingEvents.end();it++) {
                if (m_events.at(i).getPitch() == it->pitch &&
                    m_events.at(i).getChannel() == it->channel) {
                    m_noteEvents.emplace_back(it->onIndex, i);
                    pendingEvents.erase(it);
                    break;}
            }
        }
        else if (m_events.at(i).isNoteOn()) {
            pendingEvents.emplace_back(i,m_events.at(i).getPitch(),m_events.at(i).getChannel());
        }
    }
}

void Pattern::convertMidiTicksToPPQ() {
    for (auto& event : m_events) {
        uint32_t newDelta = static_cast<uint32_t>(event.getDelta()/(static_cast<float>(ticksInMidiFile))*
            SessionData::instance().getPPQ());
        event.setDelta(newDelta);
    }
}