// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Application/ApplicationBase.hpp"
#include <execution>
#include <ostream>
#include <glm/gtc/matrix_transform.hpp>
#include <luvk/Modules/CommandPool.hpp>
#include <luvk/Modules/Debug.hpp>
#include <luvk/Modules/DescriptorPool.hpp>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Memory.hpp>
#include <luvk/Modules/Renderer.hpp>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <luvk/Modules/ThreadPool.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include "luvk/Modules/Draw.hpp"

using namespace Core;

constexpr luvk::Renderer::InstanceCreationArguments g_InstArguments{.ApplicationName = "luvk-imgui-template"};

ApplicationBase::ApplicationBase(const std::uint32_t Width, const std::uint32_t Height, const SDL_WindowFlags Flags)
    : m_Width(static_cast<std::int32_t>(Width)),
      m_Height(static_cast<std::int32_t>(Height)),
      m_Renderer(luvk::CreateModule<luvk::Renderer>())
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Title = g_InstArguments.ApplicationName;

    m_Window = SDL_CreateWindow(std::data(m_Title),
                                m_Width,
                                m_Height,
                                Flags);

    m_Input = std::make_shared<InputManager>(m_Window);

    SDL_StartTextInput(m_Window);
}

bool ApplicationBase::Initialize()
{
    volkInitialize();

    RegisterModules();
    SetupInstanceExtensions();

    if (!m_Renderer->InitializeRenderer(g_InstArguments, GetInstanceFeatureChain()))
    {
        return false;
    }

    if (InitializeModules())
    {
        volkLoadInstance(m_Renderer->GetInstance());
        volkLoadDevice(m_DeviceModule->GetLogicalDevice());
        RegisterInputBindings();
        m_CanRender = true;

        return true;
    }

    return false;
}

void ApplicationBase::Shutdown()
{
    m_DeviceModule->WaitIdle();

    m_ImGuiLayer.reset();

    SDL_DestroyWindow(m_Window);
    SDL_Quit();

    m_DebugModule.reset();
    m_SwapChainModule.reset();
    m_DeviceModule.reset();
    m_MemoryModule.reset();
    m_CommandPoolModule.reset();
    m_SynchronizationModule.reset();
    m_ThreadPoolModule.reset();
    m_DescriptorPoolModule.reset();
    m_DrawModule.reset();

    m_Renderer.reset();

    volkFinalize();
}

bool ApplicationBase::Render()
{
    m_Input->ProcessEvents();
    if (!m_Input->Running())
    {
        return false;
    }

    if (!m_CanRender)
    {
        return true;
    }

    m_Width  = 0;
    m_Height = 0;
    SDL_GetWindowSizeInPixels(m_Window, &m_Width, &m_Height);

    if (m_ResizePending && m_Width > 0 && m_Height > 0)
    {
        m_Renderer->SetPaused(true);
        m_Renderer->Refresh({GetWidth(), GetHeight()});
        m_Renderer->SetPaused(false);
        m_ResizePending = false;
        m_CanRender     = true;
    }

    static auto LastTime    = std::chrono::steady_clock::now();
    const auto  CurrentTime = std::chrono::steady_clock::now();

    const float DeltaTime = std::chrono::duration<float>(CurrentTime - LastTime).count();
    m_DeltaTime           = glm::clamp(DeltaTime, 0.0001f, 0.05f);

    LastTime = CurrentTime;

    m_Renderer->DrawFrame();

    return true;
}

void ApplicationBase::SetTitle(const std::string_view Title)
{
    m_Title = Title;
    SDL_SetWindowTitle(m_Window, std::data(m_Title));
}

void ApplicationBase::PreRenderCallback([[maybe_unused]] const VkCommandBuffer CommandBuffer)
{
    m_ImGuiLayer->Draw();
}

void ApplicationBase::DrawCallback(const VkCommandBuffer CommandBuffer, const std::uint32_t CurrentFrame)
{
    m_ImGuiLayer->Render(CommandBuffer, CurrentFrame);
}

void ApplicationBase::UserEventCallback(const SDL_Event& Event)
{
    [[maybe_unused]] auto _ = m_ImGuiLayer && m_ImGuiLayer->ProcessEvent(Event);
}

void ApplicationBase::SetupInstanceExtensions() const
{
    std::uint32_t NumExtensions = 0U;

    char const* const* RawExtensionsData = SDL_Vulkan_GetInstanceExtensions(&NumExtensions);

    if (NumExtensions == 0 || RawExtensionsData == nullptr)
    {
        return;
    }

    std::vector ExtensionsData(RawExtensionsData, RawExtensionsData + NumExtensions);

    for (char const* Ext : ExtensionsData)
    {
        m_Renderer->GetExtensions().SetExtensionState("", Ext, true);
    }
}

void* ApplicationBase::GetInstanceFeatureChain() const
{
    return nullptr;
}

void ApplicationBase::SetupDeviceExtensions() const {}

void* ApplicationBase::GetDeviceFeatureChain() const
{
    return nullptr;
}

void ApplicationBase::PostRegisterImGuiLayer()
{
    m_ImGuiLayer->PushStyle();

    m_DrawModule->SetPreRenderCallback([this](const VkCommandBuffer CommandBuffer)
    {
        PreRenderCallback(CommandBuffer);
    });

    m_DrawModule->SetDrawCallback([this](const VkCommandBuffer CommandBuffer)
    {
        DrawCallback(CommandBuffer, static_cast<std::uint32_t>(m_SynchronizationModule->GetCurrentFrame()));
    });
}

void ApplicationBase::RegisterInputBindings()
{
    m_Input->BindEvent(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
                       [this]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_ResizePending = true;
                       });

    m_Input->BindEvent(SDL_EVENT_WINDOW_MINIMIZED,
                       [this]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_Renderer->SetPaused(true);
                           m_CanRender = false;
                       });

    m_Input->BindEvent(SDL_EVENT_WINDOW_RESTORED,
                       [this]([[maybe_unused]] const SDL_Event& Event)
                       {
                           m_Renderer->SetPaused(false);
                           m_CanRender = true;
                       });

    m_Input->BindEvent(SDL_EVENT_USER,
                       [this](const SDL_Event& Event)
                       {
                           UserEventCallback(Event);
                       });
}

void ApplicationBase::RegisterModules()
{
    m_DebugModule           = luvk::CreateModule<luvk::Debug>(m_Renderer);
    m_DeviceModule          = luvk::CreateModule<luvk::Device>(m_Renderer);
    m_MemoryModule          = luvk::CreateModule<luvk::Memory>(m_Renderer, m_DeviceModule);
    m_SwapChainModule       = luvk::CreateModule<luvk::SwapChain>(m_DeviceModule, m_MemoryModule);
    m_CommandPoolModule     = luvk::CreateModule<luvk::CommandPool>(m_DeviceModule);
    m_SynchronizationModule = luvk::CreateModule<luvk::Synchronization>(m_DeviceModule, m_SwapChainModule, m_CommandPoolModule);
    m_ThreadPoolModule      = luvk::CreateModule<luvk::ThreadPool>();
    m_DescriptorPoolModule  = luvk::CreateModule<luvk::DescriptorPool>(m_DeviceModule);
    m_DrawModule            = luvk::CreateModule<luvk::Draw>(m_DeviceModule, m_SwapChainModule, m_SynchronizationModule);

    m_Renderer->RegisterModules({.DebugModule = m_DebugModule,
                                 .DeviceModule = m_DeviceModule,
                                 .MemoryModule = m_MemoryModule,
                                 .SwapChainModule = m_SwapChainModule,
                                 .CommandPoolModule = m_CommandPoolModule,
                                 .SynchronizationModule = m_SynchronizationModule,
                                 .ThreadPoolModule = m_ThreadPoolModule,
                                 .DescriptorPoolModule = m_DescriptorPoolModule,
                                 .DrawModule = m_DrawModule});
}

bool ApplicationBase::InitializeModules() const
{
    if (!InitializeDeviceModule())
    {
        return false;
    }

    m_MemoryModule->InitializeAllocator(0);

    m_SwapChainModule->CreateSwapChain({.PresentMode = VK_PRESENT_MODE_FIFO_KHR,
                                        .Extent = {GetWidth(), GetHeight()},
                                        .Surface = m_DeviceModule->GetSurface()},
                                       nullptr);

    m_CommandPoolModule->CreateCommandPool(m_DeviceModule->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value(),
                                           VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    m_ThreadPoolModule->Start(std::thread::hardware_concurrency());

    constexpr std::array Sizes{VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024},
                               VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024},
                               VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024}};

    m_DescriptorPoolModule->CreateDescriptorPool(1000, Sizes);

    m_SynchronizationModule->Initialize();
    m_SynchronizationModule->SetupFrames();

    return true;
}

bool ApplicationBase::InitializeDeviceModule() const
{
    VkSurfaceKHR Surface = VK_NULL_HANDLE;
    if (!SDL_Vulkan_CreateSurface(m_Window, m_Renderer->GetInstance(), nullptr, &Surface))
    {
        return false;
    }

    m_DeviceModule->SetPhysicalDevice(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    m_DeviceModule->SetSurface(Surface);

    SetupDeviceExtensions();

    std::unordered_map<std::uint32_t, std::uint32_t> DeviceQueueMap{};
    const auto&                                      QueueProperties = m_DeviceModule->GetDeviceQueueFamilyProperties();
    std::uint32_t                                    Iterator        = 0U;

    for (const auto& QueueIt : QueueProperties)
    {
        DeviceQueueMap.emplace(Iterator++, QueueIt.queueCount);
    }

    m_DeviceModule->CreateLogicalDevice(std::move(DeviceQueueMap), GetDeviceFeatureChain());
    return true;
}
