// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Application/Application.hpp"
#include <chrono>
#include <imgui.h>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Renderer.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_vulkan.h>
#include "Core/UserInterface/ImGuiLayer.hpp"

using namespace Core;

Application::Application(const std::uint32_t Width, const std::uint32_t Height)
    : ApplicationBase(Width, Height, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE)
{
    ApplicationBase::Initialize();

    m_ImGuiLayer = std::make_unique<ImGuiLayer>(m_Window,
                                                m_Renderer->GetInstance(),
                                                m_DeviceModule,
                                                m_DescriptorPoolModule,
                                                m_SwapChainModule,
                                                m_MemoryModule);

    m_ImGuiLayer->PushStyle();

    RegisterInputBindings();

    m_Renderer->SetPreRenderCallback([this]([[maybe_unused]] const VkCommandBuffer& Cmd)
    {
        m_ImGuiLayer->Draw();
    });

    m_Renderer->SetDrawCallback([this](const VkCommandBuffer& Cmd)
    {
        const auto FrameIndex = static_cast<std::uint32_t>(m_SynchronizationModule->GetCurrentFrame());
        m_ImGuiLayer->Render(Cmd, FrameIndex);
    });
}

Application::~Application()
{
    m_DeviceModule->WaitIdle();
    m_ImGuiLayer.reset();
}

Application& Application::GetInstance()
{
    static Application Instance(4, 4);
    return Instance;
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
