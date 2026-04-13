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

    for (int i = 0; i < ArrangerManager::instance().getTrackAmount(); i++) {
        {
            const juce::ScopedLock sl(midiLock);
            localBuffer[i] = midiBuffers[i];
            midiBuffers[i].clear();
        }
        auto instrument = getInstrumentBasedOnTrack(i);
        instrument = (instrument == nullptr) ? InstrumentList[0].get() : instrument;
        // if (instrument == nullptr)
        // {
        //     instrument = InstrumentList[i].get();
        // }
        //
        std::cout << "Rendering track " << i << std::endl;
        instrument->synth->
        renderNextBlock(*bufferToFill.buffer, localBuffer[i],
                          bufferToFill.startSample,
                          bufferToFill.numSamples);;
        std::cout << "after Rendering track " << i << std::endl;
    }
}