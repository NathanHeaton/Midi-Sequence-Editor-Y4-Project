#include "Pattern.h"
#include "../SessionData.h"
#include <iostream>
//
// Created by nathan on 11/01/2026.
//
void Pattern::setLastBar() {
    auto* s = &SessionData::instance();
    std::cout << m_events.size() << std::endl;
    std::cout << m_noteEvents.end()->offIndex << std::endl;
    if (!m_noteEvents.empty()) {
        int endDelta = m_events.at(m_noteEvents.size()-1).m_absoluteTime;
        std::cout << endDelta << std::endl;
        m_bars = (ticksPerQuarterNote /endDelta) * s->getPPQ();
    }


    m_bars = m_events.size();
}

auto& Pattern::getSession() {
    auto* s = &SessionData::instance();
    return s;
}