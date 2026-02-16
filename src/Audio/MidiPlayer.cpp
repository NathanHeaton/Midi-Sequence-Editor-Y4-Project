
#include "MidiPlayer.h"

void MidiPlayer::setPlaying(bool playing) {
    if (m_playingPtr) {
        *m_playingPtr = playing;
    }
}

bool MidiPlayer::isPlaying() const {
    return m_playingPtr ? *m_playingPtr : false;
}