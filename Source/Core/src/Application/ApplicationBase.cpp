/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#include "Core/Application/ApplicationBase.hpp"
#include <execution>
#include <ostream>
#include <glm/gtc/matrix_transform.hpp>
#include <luvk/Modules/CommandPool.hpp>
#include <luvk/Modules/Debug.hpp>
#include <luvk/Modules/DescriptorPool.hpp>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Draw.hpp>
#include <luvk/Modules/Memory.hpp>
#include <luvk/Modules/Renderer.hpp>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <luvk/Modules/ThreadPool.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>

using namespace Core;

constexpr luvk::InstanceCreationArguments g_InstArguments{.ApplicationName = "overlay-exp"};

ApplicationBase::ApplicationBase(const std::uint32_t Width, const std::uint32_t Height, const SDL_WindowFlags Flags)
    : m_Width(static_cast<std::int32_t>(Width)),
      m_Height(static_cast<std::int32_t>(Height)),
      m_Renderer(luvk::CreateModule<luvk::Renderer>())
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Title = g_InstArguments.ApplicationName;
    m_Window = SDL_CreateWindow(std::data(m_Title), m_Width, m_Height, Flags);
    m_Input = std::make_shared<InputManager>(m_Window);

    SDL_StartTextInput(m_Window);
}

bool ApplicationBase::Initialize()
{
    RegisterModules();
    SetupInstanceExtensions();

    if (!m_Renderer->InitializeRenderer(g_InstArguments, GetInstanceFeatureChain()))
    {
        return false;
    }

    if (InitializeModules())
    {
        RegisterInputBindings();
        m_CanRender = true;

        return true;
    }

    return false;
}

void ApplicationBase::Shutdown()
{
    const luvk::RenderModules& Modules = m_Renderer->GetModules();
    Modules.DeviceModule->WaitIdle();

    m_EventHandles.clear();

    m_ImGuiLayer.reset();
    SDL_DestroyWindow(m_Window);
    SDL_Quit();

    m_Renderer->ClearResources();
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
        m_Renderer->Refresh({GetWidth(),
                             GetHeight()});
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

void ApplicationBase::SetTitle(const std::string_view Title) noexcept
{
    m_Title = Title;
    SDL_SetWindowTitle(m_Window, std::data(m_Title));
}

bool ApplicationBase::OnBeginFrame([[maybe_unused]] const VkCommandBuffer CommandBuffer)
{
    m_ImGuiLayer->Draw();
    return true;
}

bool ApplicationBase::OnRecordFrame(const VkCommandBuffer CommandBuffer, const std::uint32_t CurrentFrame)
{
    m_ImGuiLayer->Render(CommandBuffer, CurrentFrame);
    return true;
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

    const luvk::RenderModules& Modules = m_Renderer->GetModules();

    Modules.DrawModule->RegisterOnBeginFrame(luvk::DrawCallbackInfo([this](const VkCommandBuffer CommandBuffer)
    {
        return OnBeginFrame(CommandBuffer);
    }));

    Modules.DrawModule->RegisterOnRecordFrame(luvk::DrawCallbackInfo([this](const VkCommandBuffer CommandBuffer)
    {
        return OnRecordFrame(CommandBuffer, m_Renderer->GetCurrentFrame());
    }));
}

void ApplicationBase::RegisterInputBindings()
{
    m_EventHandles.emplace_back(m_Input->BindEvent(SDL_EVENT_WINDOW_RESIZED,
                                                   [this]([[maybe_unused]] const SDL_Event& Event, SDL_Window* Window)
                                                   {
                                                       if (Window == m_Window)
                                                       {
                                                        m_ResizePending = true;
                                                       }
                                                   }));

    m_EventHandles.emplace_back(m_Input->BindEvent(SDL_EVENT_WINDOW_MINIMIZED,
                                                   [this]([[maybe_unused]] const SDL_Event& Event, SDL_Window* Window)
                                                   {
                                                       if (Window == m_Window)
                                                       {
                                                           m_Renderer->SetPaused(true);
                                                           m_CanRender = false;
                                                       }
                                                   }));

    m_EventHandles.emplace_back(m_Input->BindEvent(SDL_EVENT_WINDOW_RESTORED,
                                                   [this]([[maybe_unused]] const SDL_Event& Event, SDL_Window* Window)
                                                   {
                                                       if (Window == m_Window)
                                                       {
                                                           m_Renderer->SetPaused(false);
                                                           m_CanRender = true;
                                                       }
                                                   }));

    m_EventHandles.emplace_back(m_Input->BindEvent(SDL_EVENT_USER,
                                                   [this](const SDL_Event& Event, [[maybe_unused]] SDL_Window* Window)
                                                   {
                                                       UserEventCallback(Event);
                                                   }));
}

void ApplicationBase::RegisterModules()
{
    const auto DebugModule           = luvk::CreateModule<luvk::Debug>(m_Renderer);
    const auto DeviceModule          = luvk::CreateModule<luvk::Device>(m_Renderer);
    const auto MemoryModule          = luvk::CreateModule<luvk::Memory>(m_Renderer, DeviceModule);
    const auto CommandPoolModule     = luvk::CreateModule<luvk::CommandPool>(DeviceModule);
    const auto SynchronizationModule = luvk::CreateModule<luvk::Synchronization>(DeviceModule);
    const auto SwapChainModule       = luvk::CreateModule<luvk::SwapChain>(DeviceModule, MemoryModule, SynchronizationModule);
    const auto ThreadPoolModule      = luvk::CreateModule<luvk::ThreadPool>();
    const auto DescriptorPoolModule  = luvk::CreateModule<luvk::DescriptorPool>(DeviceModule);
    const auto DrawModule            = luvk::CreateModule<luvk::Draw>(DeviceModule, SynchronizationModule);

    m_Renderer->RegisterModules({.DebugModule           = DebugModule,
                                 .DeviceModule          = DeviceModule,
                                 .MemoryModule          = MemoryModule,
                                 .SwapChainModule       = SwapChainModule,
                                 .CommandPoolModule     = CommandPoolModule,
                                 .SynchronizationModule = SynchronizationModule,
                                 .ThreadPoolModule      = ThreadPoolModule,
                                 .DescriptorPoolModule  = DescriptorPoolModule,
                                 .DrawModule            = DrawModule});
}

bool ApplicationBase::InitializeModules() const
{
    if (!InitializeDeviceModule())
    {
        return false;
    }

    const luvk::RenderModules& Modules = m_Renderer->GetModules();

    Modules.MemoryModule->InitializeAllocator(0);

    Modules.SwapChainModule->CreateSwapChain({.PresentMode = VK_PRESENT_MODE_FIFO_KHR,
                                              .Extent      = {GetWidth(),
                                                              GetHeight()},
                                              .Surface = Modules.DeviceModule->GetSurface()},
                                             nullptr);

    Modules.CommandPoolModule->CreateCommandPool(Modules.DeviceModule->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value(),
                                                 VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    Modules.ThreadPoolModule->Start(std::thread::hardware_concurrency());

    constexpr std::array Sizes{VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                    1024},
                               VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                    1024},
                               VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                                    1024}};

    Modules.DescriptorPoolModule->CreateDescriptorPool(1000, Sizes);
    Modules.SynchronizationModule->Initialize(Modules.CommandPoolModule->AllocateRenderCommandBuffers());

    return true;
}

bool ApplicationBase::InitializeDeviceModule() const
{
    VkSurfaceKHR Surface = VK_NULL_HANDLE;
    if (!SDL_Vulkan_CreateSurface(m_Window, m_Renderer->GetInstance(), nullptr, &Surface))
    {
        return false;
    }

    const luvk::RenderModules& Modules = m_Renderer->GetModules();

    Modules.DeviceModule->SetPhysicalDevice(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    Modules.DeviceModule->SetSurface(Surface);

    SetupDeviceExtensions();

    std::unordered_map<std::uint32_t, std::uint32_t> DeviceQueueMap{};
    const auto&                                      QueueProperties = Modules.DeviceModule->GetDeviceQueueFamilyProperties();
    std::uint32_t                                    Iterator        = 0U;

    for (const auto& QueueIt : QueueProperties)
    {
        DeviceQueueMap.emplace(Iterator++, QueueIt.queueCount);
    }

    Modules.DeviceModule->CreateLogicalDevice(std::move(DeviceQueueMap), GetDeviceFeatureChain());
    return true;
}
