#include "Pattern.h"

#include <algorithm>
#include "../Singletons/TimeData.h"
#include "../Singletons/ViewState.h"
#include <cmath>
#include <ranges>
#include <sys/stat.h>
#include "../Singletons/ToolManager.h"
#include "../Singletons/PatternManager.h"

// ============================================================
//  Helpers
// ============================================================

void Pattern::setLastBar() {
    auto barSize   = TimeData::instance().PPQ * TimeData::instance().timeSignature.getNumerator();
    int endAbsolute = m_events.back().m_absoluteTime;
    m_bars = static_cast<int>(std::ceil(static_cast<float>(endAbsolute) / static_cast<float>(barSize)));
}

void Pattern::updateBarCount(uint32_t endAbsolute) {
    if (endAbsolute >= barSizeTicks * m_bars)
        setLastBar();
}

void Pattern::updatePitchRange() {
    m_pitchRange = {};
    for (const auto& pair : m_noteEvents) {
        uint8_t pitch        = getMidiEventByID_ptr(pair.onID)->getPitch();
        m_pitchRange.lowest  = std::min(m_pitchRange.lowest,  pitch);
        m_pitchRange.highest = std::max(m_pitchRange.highest, pitch);
    }
}

// ============================================================
//  Event list management
// ============================================================

// Rebuilds m_noteEvents by walking m_events in order and pairing
// every note-on with its matching note-off by pitch + channel.
void Pattern::createNoteEventPairs() {
    if (m_events.empty()) return;

    struct PendingNoteEvent {
        uint8_t pitch, channel;
        size_t  onID;
        PendingNoteEvent(size_t id, uint8_t p, uint8_t c) : pitch(p), channel(c), onID(id) {}
    };

    std::vector<PendingNoteEvent> pending;
    int cumulativeTime = 0;

    for (auto& event : m_events) {
        cumulativeTime      += event.getDelta();
        event.m_absoluteTime = cumulativeTime;

        if (event.isNoteOff() || (event.isNoteOn() && event.getVelocity() == 0)) {
            for (auto it = pending.begin(); it != pending.end(); ++it) {
                if (event.getPitch() == it->pitch && event.getChannel() == it->channel) {
                    m_noteEvents.emplace_back(it->onID, event.getID());
                    pending.erase(it);
                    break;
                }
            }
        } else if (event.isNoteOn()) {
            pending.emplace_back(event.getID(), event.getPitch(), event.getChannel());
        }
    }

    rebuildNoteIndices();
    updatePitchRange();
}

// Inserts a MidiEvent into the sorted m_events array, adjusting
// neighbouring deltas to keep the sequence consistent.
void Pattern::insertEvent(MidiEvent& event, uint32_t absoluteTime) {
    size_t   index = findInsertionPoint(absoluteTime);
    uint32_t delta = calculateDelta(index, absoluteTime);
    event.setDelta(delta);
    event.m_absoluteTime = absoluteTime;
    m_events.insert(m_events.begin() + index, event);
    updateBarCount(absoluteTime);
}

// Removes both events of a pair, patching the deltas of their
// immediate successors so the rest of the sequence is unaffected.
void Pattern::removeNoteOperation(NoteEventPair notepair) {
    auto onNote = getMidiEventByID_ptr(notepair.onID);
    auto offNote = getMidiEventByID_ptr(notepair.offID);
    auto onDelta = onNote->getDelta();

    auto noteOnIndex = getEventIndexByID(notepair.onID);
    auto noteOffIndex = getEventIndexByID(notepair.offID);

    if (noteOnIndex+1 < m_events.size()) {
        m_events[noteOnIndex+1].setDelta(m_events[noteOnIndex+1].getDelta() +onDelta );
    }
    auto offDelta = offNote->getDelta();
    if (noteOffIndex +1< m_events.size()) {
        m_events[noteOffIndex+1].setDelta(m_events[noteOffIndex+1].getDelta() +offDelta );
    }

    size_t first  = std::min(noteOnIndex, noteOffIndex);
    size_t second = std::max(noteOnIndex, noteOffIndex);

    m_events.erase(m_events.begin() + second);
    m_events.erase(m_events.begin() + first);

}

size_t Pattern::findInsertionPoint(uint32_t absoluteTime) {
    size_t index = 0;
    for (; index < m_events.size(); index++) {
        if (m_events[index].getAbsoluteTime() >= absoluteTime) break;
    }
    if (index < m_events.size()) {
        m_events[index].setDelta(m_events[index].getAbsoluteTime() - absoluteTime);
    }
    return index;
}

uint32_t Pattern::calculateDelta(size_t insertionIndex, uint32_t absoluteTime) const {
    if (insertionIndex == 0 || m_events.empty()) return absoluteTime;
    return absoluteTime - m_events[insertionIndex - 1].getAbsoluteTime();
}


// ============================================================
//  Note operations
// ============================================================

void Pattern::addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t duration) {
    m_previousOperationsOnNoteIDs.clear();
    const uint8_t  channel        = 0;
    const uint8_t  velocity       = 127;
    const uint32_t offAbsoluteTime = absoluteTime + duration;

    MidiEvent noteOn(0, 0x90, Note{t_pitch, velocity}, channel);
    noteOn.setID(assignID());
    MidiEvent noteOff(0, 0x80, Note{t_pitch, velocity}, channel);
    noteOff.setID(assignID());
    m_previousOperationsOnNoteIDs.insert(noteOn.getID());
    insertEvent(noteOn,  absoluteTime);
    insertEvent(noteOff, offAbsoluteTime);

    m_noteEvents.clear();
    createNoteEventPairs();
    overlapValidate();
}

void Pattern::moveNoteEvent(uint32_t ID, NoteCoordinate pos) {
    auto pair = findPairByID(ID);
    executeOperation({pair}, [&](MidiEvent& on, MidiEvent& off) {
        uint32_t duration = off.getAbsoluteTime() - on.getAbsoluteTime();
        on.setPitch(pos.pitch);
        off.setPitch(pos.pitch);
        on.setAbsoluteTime(pos.absoluteTime);
        off.setAbsoluteTime(pos.absoluteTime + duration);
    });
}

void Pattern::stretchNoteEvent(uint32_t ID, int32_t endDelta) {
    auto pair = findPairByID(ID);
    executeOperation({pair}, [&](MidiEvent& on, MidiEvent& off) {
        uint32_t newEnd = off.getAbsoluteTime() + endDelta;
        if (newEnd <= on.getAbsoluteTime())
            newEnd = on.getAbsoluteTime() + ViewState::instance().getStandardSnapTime();
        off.setAbsoluteTime(newEnd);
    });
}

void Pattern::stretchNoteEventSelection(NoteMoveDelta delta) {
    auto pairs = convertNoteIdsToNotePair();
    executeOperation(pairs, [&](MidiEvent& on, MidiEvent& off) {
        uint32_t newEnd = off.getAbsoluteTime() + delta.timeDelta;
        if (newEnd <= on.getAbsoluteTime())
            newEnd = on.getAbsoluteTime() + ViewState::instance().getStandardSnapTime();
        off.setAbsoluteTime(newEnd);
    });
}

void Pattern::moveNoteEventSelection(NoteMoveDelta delta) {
    std::cout<<"begun move selection"<<std::endl;
    auto pairs = convertNoteIdsToNotePair();
    executeOperation(pairs, [&](MidiEvent& on, MidiEvent& off) {
        if (static_cast<int32_t>(on.getAbsoluteTime()) + delta.timeDelta < 0) return;
        std::cout<<on.getAbsoluteTime()<< " : " << off.getAbsoluteTime() << "time pre change"<<std::endl;
        on.setAbsoluteTime(on.getAbsoluteTime()   + delta.timeDelta);
        off.setAbsoluteTime(off.getAbsoluteTime() + delta.timeDelta);

        std::cout<<on.getPitch()<< " : " << off.getPitch() << "pitch pre change"<<std::endl;
        on.setPitch( std::clamp(on.getPitch()  + delta.pitchD, 0, 127));
        off.setPitch(std::clamp(off.getPitch() + delta.pitchD, 0, 127));


    });
    m_selectedNoteOnIDs.clear();
    std::cout<<"ended move selection"<<std::endl;
}

void Pattern::scaleNoteEventSelection(float scale) {
    if (m_selectedNoteOnIDs.empty() || scale <= 0.2f) return;

    auto pairs = convertNoteIdsToNotePair();

    // Find the earliest note start to use as the scale anchor
    uint32_t anchor = UINT32_MAX;
    for (auto& p : pairs)
        anchor = std::min(anchor, getMidiEventByID_ptr(p.onID)->getAbsoluteTime());

    executeOperation(pairs, [&](MidiEvent& on, MidiEvent& off) {
        on.setAbsoluteTime( anchor + static_cast<uint32_t>((on.getAbsoluteTime()  - anchor) * scale));
        off.setAbsoluteTime(anchor + static_cast<uint32_t>((off.getAbsoluteTime() - anchor) * scale));
    });
    m_selectedNoteOnIDs.clear();
}

// Pitch-only shift — no repositioning needed so bypasses executeOperation
void Pattern::pitchShiftSelection(signed short pitchDelta) {
    m_previousOperationsOnNoteIDs.clear();

    for (const auto& pair : m_noteEvents) {
        if (!m_selectedNoteOnIDs.contains(pair.onID)) continue;
        auto* on      = getMidiEventByID_ptr(pair.onID);
        int newPitch  = std::clamp(on->getPitch() + pitchDelta, 0, 127);
        on->setPitch(newPitch);
        getMidiEventByID_ptr(pair.offID)->setPitch(newPitch);
        m_previousOperationsOnNoteIDs.insert(on->getID());
    }
    updatePitchRange();
    overlapValidate();
}

// ============================================================
//  Selection & removal
// ============================================================

void Pattern::removeNote(NoteEventPair note) {
    if (m_events.empty()) return;
    removeNoteOperation(note);
    m_selectedNoteOnIDs.clear();
    m_hiddenNoteOnIDs.clear();
}

void Pattern::removeSelection(std::vector<NoteCoordinate> events) {
    for (auto& event : events) {
        auto notePair = findNoteBasedOnPoint(event);
        if (!notePair || getEventIndexByID(notePair->offID) == SIZE_MAX) return;
        removeNote(*notePair);
    }
    m_selectedNoteOnIDs.clear();
    m_hiddenNoteOnIDs.clear();
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::deleteSelection() {
    if (m_selectedNoteOnIDs.empty()) return;
    for (auto& noteIds : m_noteEvents) {
        if (m_selectedNoteOnIDs.contains(noteIds.onID))
            removeNoteOperation(noteIds);
    }
    m_selectedNoteOnIDs.clear();
    m_noteEvents.clear();
    createNoteEventPairs();
}

// ============================================================
//  Selection query
// ============================================================

void Pattern::calculateSelection(const SelectionCoords& t_selection) {
    m_selectedNoteOnIDs.clear();

    float pixelPerTick = TimeData::instance().PPQ / ViewState::instance().getPixelPerBeat(zoomFactor::pianoRoll);
    float noteHeight   = ViewState::instance().getNoteHeight();

    ImVec2 p1 = { t_selection.selectP1.x * pixelPerTick, t_selection.selectP1.y / noteHeight };
    ImVec2 p2 = { t_selection.selectP2.x * pixelPerTick, t_selection.selectP2.y / noteHeight };

    for (auto& note : m_noteEvents) {
        float onX  = static_cast<float>(getMidiEventByID_ptr(note.onID)->getAbsoluteTime());
        float offX = static_cast<float>(getMidiEventByID_ptr(note.offID)->getAbsoluteTime());
        float onY  = 127.0f - static_cast<float>(getMidiEventByID_ptr(note.onID)->getPitch());

        if ((onX > p1.x || offX > p1.x) && onX < p2.x)
            if (onY > p1.y && onY + 1 < p2.y)
                m_selectedNoteOnIDs.emplace(note.onID);
    }
}

// ============================================================
//  Hover detection
// ============================================================

HoverState Pattern::findNoteHoverState(NoteCoordinate hover) {
    for (const auto& pair : m_noteEvents) {
        const auto* on  = getMidiEventByID_ptr(pair.onID);
        const auto* off = getMidiEventByID_ptr(pair.offID);
        if (on->getPitch() != hover.pitch) continue;
        if (on->getAbsoluteTime() > hover.absoluteTime || off->getAbsoluteTime() < hover.absoluteTime) continue;

        // Outer 20% of the note triggers edge hover
        uint32_t edgeThreshold = off->getAbsoluteTime() -
            static_cast<uint32_t>((off->getAbsoluteTime() - on->getAbsoluteTime()) * 0.2f);
        return hover.absoluteTime <= edgeThreshold ? CenterHover : EdgeHover;
    }
    return NoHover;
}


// ============================================================
//  Miscellaneous
// ============================================================

std::optional<NoteEventPair> Pattern::findNoteBasedOnPoint(NoteCoordinate coord) {
    if (m_events.empty()) return std::nullopt;
    for (const auto& pair : m_noteEvents) {
        auto* on  = getMidiEventByID_ptr(pair.onID);
        auto* off = getMidiEventByID_ptr(pair.offID);
        if (on->getPitch() == coord.pitch &&
            on->getAbsoluteTime()  <= coord.absoluteTime &&
            off->getAbsoluteTime() >= coord.absoluteTime)
            return pair;
    }
    return std::nullopt;
}

void Pattern::hideNoteEvent(NoteCoordinate coordinate) {
    for (const auto& pair : m_noteEvents) {
        const auto* on  = getMidiEventByID_ptr(pair.onID);
        const auto* off = getMidiEventByID_ptr(pair.offID);
        if (on->getPitch() == coordinate.pitch &&
            on->getAbsoluteTime()  <= coordinate.absoluteTime &&
            off->getAbsoluteTime() >= coordinate.absoluteTime)
            m_hiddenNoteOnIDs.insert(pair.onID);
    }
}

void Pattern::setNoteVelocity(NoteEventPair notePair, uint8_t velocity) {
    getMidiEventByID_ptr(notePair.onID)->setVelocity(velocity);
    getMidiEventByID_ptr(notePair.offID)->setVelocity(velocity);
}

void Pattern::convertMidiTicksToPPQ() {
    for (auto& event : m_events) {
        uint32_t newDelta = static_cast<uint32_t>(
            event.getDelta() / static_cast<float>(ticksInMidiFile) * TimeData::instance().PPQ);
        event.setDelta(newDelta);
    }
}

void Pattern::pasteClipboard(std::vector<MidiEvent>& copied_events, NoteMoveDelta offset) {
    m_selectedNoteOnIDs.clear();
    for (auto event : copied_events) {
        event.setID(assignID());
        if (event.isNoteOn()) m_selectedNoteOnIDs.emplace(event.getID());
        event.setPitch(event.getPitch() + offset.pitchD);
        event.setAbsoluteTime(event.getAbsoluteTime() + offset.timeDelta);
        insertEvent(event, event.getAbsoluteTime());
    }
    m_noteEvents.clear();
    createNoteEventPairs();
    overlapValidate();
}

// Legacy — kept for keyboard nudge shortcuts, delegates to executeOperation
void Pattern::timeShiftSelection(int32_t timeDelta) {
    if (m_selectedNoteOnIDs.empty()) return;
    auto pairs = convertNoteIdsToNotePair();
    executeOperation(pairs, [&](MidiEvent& on, MidiEvent& off) {
        if (static_cast<int32_t>(on.getAbsoluteTime()) + timeDelta < 0) return;
        on.setAbsoluteTime(on.getAbsoluteTime()   + timeDelta);
        off.setAbsoluteTime(off.getAbsoluteTime() + timeDelta);
    });
}