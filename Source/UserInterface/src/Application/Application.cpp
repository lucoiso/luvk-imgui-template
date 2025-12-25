// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Application/Application.hpp"
#include <imgui.h>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Draw.hpp>
#include <SDL3/SDL_events.h>

using namespace UserInterface;

std::shared_ptr<Application> Application::s_Instance = nullptr;

Application::Application(const std::uint32_t Width, const std::uint32_t Height)
    : ApplicationBase(Width, Height, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE) {}

bool Application::Initialize()
{
    volkInitialize();

    if (ApplicationBase::Initialize())
    {
        volkLoadInstance(s_Instance->m_Renderer->GetInstance());
        volkLoadDevice(s_Instance->m_DeviceModule->GetLogicalDevice());
        return true;
    }

    return false;
}

void Application::Shutdown()
{
    ApplicationBase::Shutdown();
    s_Instance.reset();
}

std::shared_ptr<Application> Application::GetInstance()
{
    if (!s_Instance)
    {
        s_Instance = std::shared_ptr<Application>(new Application(4, 4),
                                                  [](const Application* Instance)
                                                  {
                                                      delete Instance;
                                                      volkFinalize();
                                                  });
    }

    return s_Instance;
}