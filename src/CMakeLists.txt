set(JUMAKE_TEMPLATE "GuiApplication")
juce_add_gui_app(${PROJECT_NAME}
        PRODUCT_NAME ${PROJECT_NAME})

# Add imgui_impl_juce as a JUCE module
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui_juce/modules/imgui_impl_juce)

target_sources(${PROJECT_NAME}
        PRIVATE
        MIDI_Logic/MIDI_Events/MidiEvent.cpp
        MIDI_Logic/filePicker.cpp
        MIDI_Logic/MidiFileManager.cpp
        MIDI_Logic/MidiByteReader.cpp
        MIDI_Logic/ParsedMidi.cpp
        MIDI_Logic/TrackChunk.cpp
        Misc/MyColours.cpp
        Main.cpp)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_gui_extra
        juce::juce_opengl
        imgui  # Just link imgui, not imgui_impl_juce
        imgui_impl_juce  # This is now a JUCE module
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)