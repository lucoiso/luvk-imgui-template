// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Application/ApplicationBase.hpp"
#include <execution>
#include <ostream>
#include <glm/gtc/matrix_transform.hpp>
#include <luvk/Libraries/ShaderCompiler.hpp>
#include <luvk/Modules/CommandPool.hpp>
#include <luvk/Modules/Debug.hpp>
#include <luvk/Modules/DescriptorPool.hpp>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Memory.hpp>
#include <luvk/Modules/Renderer.hpp>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <luvk/Modules/ThreadPool.hpp>
#include <luvk/Types/Vector.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>

using namespace Core;

constexpr luvk::Renderer::InstanceCreationArguments g_InstArguments{.ApplicationName = "LuVK Example",
                                                                    .EngineName = "luvk",
                                                                    .ApplicationVersion = VK_MAKE_VERSION(0U, 0U, 1U),
                                                                    .EngineVersion = VK_MAKE_VERSION(0U, 0U, 1U)};

ApplicationBase::ApplicationBase(const std::uint32_t Width, const std::uint32_t Height, const SDL_WindowFlags Flags)
    : m_Width(static_cast<std::int32_t>(Width)),
      m_Height(static_cast<std::int32_t>(Height)),
      m_Renderer(luvk::CreateModule<luvk::Renderer>())
{
    volkInitialize();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Title = g_InstArguments.ApplicationName;

    m_Window = SDL_CreateWindow(std::data(m_Title),
                                m_Width,
                                m_Height,
                                Flags);

    m_Input = std::make_shared<InputManager>(m_Window);

    SDL_StartTextInput(m_Window);
}

ApplicationBase::~ApplicationBase()
{
    m_DeviceModule->WaitIdle();
    luvk::ShutdownShaderCompiler();

    SDL_DestroyWindow(m_Window);
    SDL_Quit();

    volkFinalize();
}

bool ApplicationBase::Initialize()
{
    RegisterModules();
    SetupExtensions();

    if (!m_Renderer->InitializeRenderer(g_InstArguments, nullptr))
    {
        return false;
    }

    if (InitializeModules())
    {
        volkLoadInstance(m_Renderer->GetInstance());
        volkLoadDevice(m_DeviceModule->GetLogicalDevice());
        luvk::InitializeShaderCompiler();
        m_Renderer->InitializeRenderLoop();
        m_CanRender = true;

        return true;
    }

    return false;
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

void ApplicationBase::RegisterModules()
{
#ifdef _DEBUG
    m_DebugModule = luvk::CreateModule<luvk::Debug>(m_Renderer);
#else
    m_DebugModule = nullptr;
#endif

    m_DeviceModule          = luvk::CreateModule<luvk::Device>(m_Renderer);
    m_MemoryModule          = luvk::CreateModule<luvk::Memory>(m_Renderer, m_DeviceModule);
    m_SwapChainModule       = luvk::CreateModule<luvk::SwapChain>(m_DeviceModule, m_MemoryModule);
    m_CommandPoolModule     = luvk::CreateModule<luvk::CommandPool>(m_DeviceModule);
    m_SynchronizationModule = luvk::CreateModule<luvk::Synchronization>(m_DeviceModule, m_SwapChainModule, m_CommandPoolModule);
    m_ThreadPoolModule      = luvk::CreateModule<luvk::ThreadPool>();
    m_DescriptorPoolModule  = luvk::CreateModule<luvk::DescriptorPool>(m_DeviceModule);

    m_Renderer->RegisterModules({.DebugModule = m_DebugModule,
                                 .DeviceModule = m_DeviceModule,
                                 .MemoryModule = m_MemoryModule,
                                 .SwapChainModule = m_SwapChainModule,
                                 .CommandPoolModule = m_CommandPoolModule,
                                 .SynchronizationModule = m_SynchronizationModule,
                                 .ThreadPoolModule = m_ThreadPoolModule,
                                 .DescriptorPoolModule = m_DescriptorPoolModule});
}

void ApplicationBase::SetupExtensions() const
{
    std::uint32_t NumExtensions = 0U;

    char const* const* RawExtensionsData = SDL_Vulkan_GetInstanceExtensions(&NumExtensions);

    if (NumExtensions == 0 || RawExtensionsData == nullptr)
    {
        return;
    }

    luvk::Vector<char const*> ExtensionsData(RawExtensionsData, RawExtensionsData + NumExtensions);

    for (char const* Ext : ExtensionsData)
    {
        m_Renderer->GetExtensions().SetExtensionState("", Ext, true);
    }
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

    constexpr luvk::Array Sizes = {VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024},
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

    auto& DevExt = m_DeviceModule->GetExtensions();
    if (DevExt.HasAvailableExtension(VK_EXT_MESH_SHADER_EXTENSION_NAME))
    {
        DevExt.SetExtensionState("", VK_EXT_MESH_SHADER_EXTENSION_NAME, true);
    }

    if (DevExt.HasAvailableExtension(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        DevExt.SetExtensionState("", VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME, true);
    }

    if (DevExt.HasAvailableExtension(VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME))
    {
        DevExt.SetExtensionState("", VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME, true);
    }

    constexpr VkPhysicalDeviceMeshShaderFeaturesEXT MeshShaderFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT,
                                                                       .taskShader = VK_TRUE,
                                                                       .meshShader = VK_TRUE};

    luvk::UnorderedMap<std::uint32_t, std::uint32_t> DeviceQueueMap{};
    const auto&                                      QueueProperties = m_DeviceModule->GetDeviceQueueFamilyProperties();
    std::uint32_t                                    Iterator        = 0U;

    for (const auto& Q : QueueProperties)
    {
        DeviceQueueMap.emplace(Iterator++, Q.queueCount);
    }

    m_DeviceModule->CreateLogicalDevice(std::move(DeviceQueueMap), &MeshShaderFeatures);
    return true;
}
