juce_add_gui_app(${PROJECT_NAME}
        PRODUCT_NAME ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        Main.cpp
        MIDI_Logic/MIDI_Events/MidiEvent.cpp
        MIDI_Logic/MidiByteReader.cpp
        MIDI_Logic/ParsedMidi.cpp
        MIDI_Logic/TrackChunk.cpp
        Project_Logic/Pattern.cpp
        UI/Arranger/Arranger.cpp
        UI/Common/ToolbarGroups.cpp
        SetupAssets.cpp
        UI/Common/TimelineLabel.cpp
        UI/Arranger/TrackControls.cpp
        Audio/AudioManager.cpp
        UI/ControlPanel/ControlComponent.cpp
        UI/Piano_Roll/Panels/PianoRoll.cpp)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        imgui_impl_juce
        juce::juce_audio_devices
        juce::juce_audio_basics
        juce::juce_audio_formats
        juce::juce_gui_extra
        juce::juce_opengl
        imgui
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

add_custom_command(TARGET myProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:myProject>/assets
        COMMENT "Copying assets to build directory"
)


