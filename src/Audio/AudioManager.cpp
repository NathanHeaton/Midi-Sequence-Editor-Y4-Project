#include "AudioManager.h"
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

void AudioManager::addMidiMessage(const juce::MidiMessage& message) {
    const juce::ScopedLock sl(midiLock);
    auto ar = ArrangerManager::instance();
    for (auto& instrument : InstrumentList){
        for (auto i{0u} ; i < ar.getTracks().size(); i++) {
            if (ar.getTrack(i)->muted) continue;
            if (ar.getTrack(i)->instrumentID == instrument->ID &&
                message.getChannel() == i + 1) {
                std::cout << "Add Midi Message: " << ar.getTrack(i)->instrumentID<< "track: " << ar.getTrack(i) << std::endl;
                instrument->midiBuffer.addEvent(message,0);
                break;
            }
        }
    }
}
