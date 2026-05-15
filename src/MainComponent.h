#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/ControlPanel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"
#include "Theme.h"
#include "backends/imgui_impl_opengl3.h"
#include <imgui_impl_juce/imgui_impl_juce.h>
#include <juce_opengl/juce_opengl.h>

#include <imgui.h>
#include  "UI/Piano_Roll/PianoRollMain.h"
#include "Singletons/PanelManager.h"


class MainComponent
  : public juce::Component
  , public juce::OpenGLRenderer {
public:
    TopNavComponent top_nav_component;
    ControlComponent control_component;
    Arranger arranger_component;
    std::deque<PianoRollMain> pianoRollPanels;

    MainComponent() {
        setOpaque(true);
        setSize(1920, 1080);
        setWantsKeyboardFocus(true);
        std::cout<<"ran attach"<<std::endl;
        glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
        glctx.setRenderer(this);
        glctx.attachTo(*this);
        glctx.setContinuousRepainting(true);
    }

    ~MainComponent() {
        glctx.detach();
    }

    void parentHierarchyChanged() override {
        // if (isShowing() && !m_contextInitialised && !glctx.isAttached()) {
        //     std::cout<<"ran attach"<<std::endl;
        //     glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
        //     glctx.setRenderer(this);
        //     glctx.attachTo(*this);
        //     glctx.setContinuousRepainting(true);
        // }
    }

    void newOpenGLContextCreated() override {
        if (m_contextInitialised.exchange(true)) return;

        juce::MessageManager::callAsync([this]() {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui_ImplJuce_Init(*this, glctx);
            // Store the context pointer so renderOpenGL can set it
            m_imguiContext = ImGui::GetCurrentContext();

            ImGuiIO& io = ImGui::GetIO();
            Theme::applyTheme();
            Theme::setImGuiStyle();
            //io.Fonts->Build();
            ASSETS.LoadAll();
            std::cout<<"ran context"<<std::endl;

            m_juceImguiReady = true;
        });
    }

    // stop juce assertion failure of juce::component:1695
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::red);
    }

    void renderOpenGL() override {
        using namespace juce::gl;
        if (!m_juceImguiReady || m_imguiContext == nullptr) return;
        ImGui::SetCurrentContext(m_imguiContext);

        if (!m_openglBackendReady) {
            bool result = ImGui_ImplOpenGL3_Init("#version 150");
            std::cout<<result<<std::endl;
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cout<<"GL Error after init: " + juce::String(err)<<std::endl;
            }

            m_openglBackendReady = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplJuce_NewFrame();
        ImGui::GetIO().DisplaySize = ImVec2((float)getWidth(), (float)getHeight());
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();
        std::cout<<"CmdListsCount: " + juce::String(drawData ? drawData->CmdListsCount : -1)<<std::endl;
        std::cout<<"TotalVtxCount: " + juce::String(drawData ? drawData->TotalVtxCount : -1)<<std::endl;

        ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout<<"GL Error after render: " + juce::String(err)<<std::endl;
        }
    }

    // void renderOpenGL() override {
    //     using namespace juce::gl;
    //     if (!m_juceImguiReady || m_imguiContext == nullptr) return;
    //
    //     ImGui::SetCurrentContext(m_imguiContext);
    //
    //     if (!m_openglBackendReady) {
    //         std::cout<<"ran baciemff"<<std::endl;
    //         ImGui_ImplOpenGL3_Init();
    //         std::cout<<"ran baciemff 3"<<std::endl;
    //         m_openglBackendReady = true;
    //         std::cout<<"Display size: " << juce::String(ImGui::GetIO().DisplaySize.x)
    //     + "x" + juce::String(ImGui::GetIO().DisplaySize.y) << std::endl;
    //         std::cout<<"Framebuffer scale: " + juce::String(ImGui::GetIO().DisplayFramebufferScale.x)<< std::endl;
    //
    //     }
    //
    //     ImGui_ImplOpenGL3_NewFrame();
    //     ImGui_ImplJuce_NewFrame();
    //     ImGui::NewFrame();
    //
    //     ImGui::SetNextWindowPos(ImVec2(0, 0));
    //     ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize));
    //
    //     ImGui::Begin("Main Window", nullptr, flags);
    //
    //     top_nav_component.nav();
    //     ImGui::Separator();
    //     control_component.ControlPanel();
    //     ImGui::Separator();
    //     arranger_component.create();
    //
    //     if (PanelManager::instance().newPaternPanelJustCreated) {
    //         //pianoRollPanels.push_back(PanelManager::instance().getPanels().at(
    //         //    PanelManager::instance().getPanels().size()-1));
    //         PanelManager::instance().newPaternPanelJustCreated = false;
    //     }
    //     for (auto& p : pianoRollPanels){
    //         if (p.panelDetails->open) {
    //             if (PatternManager::instance().getPatternByID(p.panelDetails->patternID)==nullptr) {
    //                 continue;
    //             }
    //             p.create();
    //         }
    //     }
    //
    //     ImGui::End();
    //     ImGui::Render();
    //
    //     glClearColor(0, 0, 0, 1);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // }

    void openGLContextClosing() override {
        m_juceImguiReady = false;
        m_openglBackendReady = false;

        if (m_imguiContext != nullptr) {
            ImGui::SetCurrentContext(m_imguiContext);
            ImGui_ImplOpenGL3_Shutdown();
            if (ImGui::GetIO().BackendPlatformUserData != nullptr) {
                ImGui_ImplJuce_Shutdown();
            }
            ImGui::DestroyContext();
            m_imguiContext = nullptr;
        }
    }

private:
    std::atomic<bool> m_juceImguiReady{false};
    std::atomic<bool> m_openglReady{false};
    std::atomic<bool> m_contextInitialised{false};
    std::atomic<bool> m_openglBackendReady{false};
    ImGuiContext* m_imguiContext{nullptr};

    juce::OpenGLContext glctx;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoBringToFrontOnFocus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
  };
