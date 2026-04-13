#include "AudioManager.h"
#include "../Singletons/ArrangerManager.h"

void AudioManager::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();

    std::array<juce::MidiBuffer, 16> localBuffer;

    // {
    //     const juce::ScopedLock sl(midiLock);
    //     localBuffer = midiBuffers[0];
    //     midiBuffers[0].clear();
    // }
    // InstrumentList[0]->synth->
    //         renderNextBlock(*bufferToFill.buffer, localBuffer,
    //                           bufferToFill.startSample,
    //                           bufferToFill.numSamples);;
    // TODO: if instrument is already used by another buffer figure out a solution, either make a copy for that track or use the same buffer
    for (int i = 0; i < ArrangerManager::instance().getTrackAmount(); i++) {
        {
            const juce::ScopedLock sl(midiLock);
            localBuffer[i] = midiBuffers[i];
            midiBuffers[i].clear();
        }
        auto instrument = getInstrumentBasedOnTrack(i);
        auto buffer = localBuffer[i];
        if (instrument == nullptr) {buffer = localBuffer[0];}
        instrument = (instrument == nullptr) ? InstrumentList[0].get() : instrument;

        instrument->synth->
        renderNextBlock(*bufferToFill.buffer, buffer,
                          bufferToFill.startSample,
                          bufferToFill.numSamples);;

    }
}