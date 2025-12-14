#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/ControlPannel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "Misc/MyColours.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"
#include "UI/Piano_Roll/PianoRollWindow.h"

#include <imgui.h>
#include "backends/imgui_impl_opengl3.h"

#include <imgui_impl_juce/imgui_impl_juce.h>
#include <juce_opengl/juce_opengl.h>

class MainComponent
  : public juce::Component
  , public juce::OpenGLRenderer
{
public:
    TopNavComponent top_nav_component;
    ControlComponent control_component;
    Arranger arranger_component;

    MainComponent()
    {
        setOpaque(true);
        setSize(1920, 1080);
        setWantsKeyboardFocus(true);
        glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
        glctx.setRenderer(this);
        glctx.attachTo(*this);
        glctx.setContinuousRepainting(true);
    }

    ~MainComponent() {
        glctx.detach();
    }

    void newOpenGLContextCreated() override
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplJuce_Init(*this, glctx);
        ImGui_ImplOpenGL3_Init();
    }

    void renderOpenGL() override
    {
        using namespace juce::gl;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplJuce_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize));

        ImGui::Begin("Main Window", nullptr, flags);

        top_nav_component.nav();

        control_component.ControldPanel();

        arranger_component.arranger();

        ImGui::End();

        ImGui::Render();

        // background begin
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // background end

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void openGLContextClosing() override
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplJuce_Shutdown();
        ImGui::DestroyContext();
    }


private:
    juce::OpenGLContext glctx;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoBringToFrontOnFocus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
  };
