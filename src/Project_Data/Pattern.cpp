#include "Pattern.h"
//#include "../SessionData.h"
//
// Created by nathan on 11/01/2026.
//
void Pattern::setLastBar() {
    auto endDelta = m_events.at(m_noteEvents.size()-1).m_absoluteTime;
    m_bars = (ticksPerQuarterNote /endDelta)*  m_barLength;
    m_bars = m_events.size();
}