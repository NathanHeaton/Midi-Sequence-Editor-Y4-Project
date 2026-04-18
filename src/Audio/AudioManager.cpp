#include "AudioManager.h"

#include "EventComplier.h"
#include "../Singletons/ArrangerManager.h"
#include "../Singletons/ProjectData.h"

void AudioManager::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();

    for (auto& instrument : InstrumentList) {
        instrument->audioBuffer.setSize(2, bufferToFill.numSamples, false, false, true);
        instrument->audioBuffer.clear();
        instrument->synth->renderNextBlock(instrument->audioBuffer,
                                         instrument->midiBuffer, 0, bufferToFill.numSamples);
        instrument->midiBuffer.clear();
    }

    for (const auto& track : ArrangerManager::instance().getTracks()) {
        if (track.muted) continue;
        auto& instrBuffer = InstrumentList[track.instrumentID]->audioBuffer;
        for (int ch = 0; ch < 2; ch++) {
            bufferToFill.buffer->addFromWithRamp(
                ch, bufferToFill.startSample,
                instrBuffer.getReadPointer(ch),
                bufferToFill.numSamples,
                track.previousVolume* ProjectData::instance().masterVolume, track.volume * ProjectData::instance().masterVolume
            );
        }
    }
    for (auto& track : ArrangerManager::instance().getTracks())
        track.previousVolume = track.volume;
}

void AudioManager::addMidiMessage(const ScheduledEvent& event) {

    const juce::ScopedLock sl(midiLock);
    auto message = event.message;
    auto ar = ArrangerManager::instance();
    auto trackIndex = 0;
    for (trackIndex ; trackIndex < ar.getTracks().size(); trackIndex++) {
        for (auto& instrument : InstrumentList)
        {
            if (ar.getTrack(trackIndex)->muted) { continue; }
            if (ar.getTrack(trackIndex)->instrumentID == instrument->ID &&
                event.trackIndex == trackIndex) {
                instrument->midiBuffer.addEvent(message,0);
                break;
                }
        }
    }

}
