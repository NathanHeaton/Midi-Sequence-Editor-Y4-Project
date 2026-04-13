#include "AudioManager.h"
#include "../Singletons/ArrangerManager.h"

void AudioManager::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();

    for (int i = 0; i < ArrangerManager::instance().getTrackAmount(); i++) {
        auto instrument = getInstrumentBasedOnTrack(i);
        instrument->synth->
        renderNextBlock(*bufferToFill.buffer, midiBuffers[i],
                          bufferToFill.startSample,
                          bufferToFill.numSamples);;
    }
}