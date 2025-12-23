// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Application/Application.hpp"
#include <imgui.h>
#include <luvk/Libraries/ShaderCompiler.hpp>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Draw.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_vulkan.h>

using namespace UserInterface;

std::shared_ptr<Application> Application::s_Instance = nullptr;

Application::Application(const std::uint32_t Width, const std::uint32_t Height)
    : ApplicationBase(Width, Height, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE) {}

bool Application::Initialize()
{
    volkInitialize();

    if (ApplicationBase::Initialize())
    {
        luvk::InitializeShaderCompiler();

        volkLoadInstance(s_Instance->m_Renderer->GetInstance());
        volkLoadDevice(s_Instance->m_DeviceModule->GetLogicalDevice());

        s_Instance->RegisterInputBindings();
        return true;
    }

    return false;
}

void Application::Shutdown()
{
    m_DeviceModule->WaitIdle();
    m_ImGuiLayer.reset();
    ApplicationBase::Shutdown();
}

std::shared_ptr<Application> Application::GetInstance()
{
    if (!s_Instance)
    {
        s_Instance = std::shared_ptr<Application>(new Application(4, 4),
                                                  [](const Application* Instance)
                                                  {
                                                      delete Instance;

                                                      luvk::ShutdownShaderCompiler();
                                                      volkFinalize();
                                                  });
    }

    return s_Instance;
}

void Application::PostRegisterImGuiLayer() const
{
    m_ImGuiLayer->PushStyle();

    m_DrawModule->SetPreRenderCallback([this]([[maybe_unused]] const VkCommandBuffer Cmd)
    {
        m_ImGuiLayer->Draw();
    });

    m_DrawModule->SetDrawCallback([this](const VkCommandBuffer Cmd)
    {
        m_ImGuiLayer->Render(Cmd, static_cast<std::uint32_t>(m_SynchronizationModule->GetCurrentFrame()));
    });
}

void Application::RegisterInputBindings()
{
    m_Input->BindEvent(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
                       [&]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_ResizePending = true;
                       });

    m_Input->BindEvent(SDL_EVENT_WINDOW_MINIMIZED,
                       [&]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_Renderer->SetPaused(true);
                           m_CanRender = false;
                       });

    m_Input->BindEvent(SDL_EVENT_WINDOW_RESTORED,
                       [&]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_Renderer->SetPaused(false);
                           m_CanRender = true;
                       });

    m_Input->BindEvent(SDL_EVENT_USER,
                       [&](const SDL_Event& Event)
                       {
                           [[maybe_unused]] auto _ = m_ImGuiLayer && m_ImGuiLayer->ProcessEvent(Event);
                       });
}
