// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/UserInterface/Backend/ImGuiBackendVulkan.hpp"
#include <luvk/Modules/CommandPool.hpp>
#include <luvk/Modules/DescriptorPool.hpp>
#include <luvk/Modules/Device.hpp>
#include <luvk/Modules/Memory.hpp>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Modules/Synchronization.hpp>
#include <luvk/Modules/Draw.hpp>
#include "Core/Meshes/ImGuiMesh.hpp"

#undef CreateWindow

using namespace Core;

static VkQueue GetGraphicsQueue(const std::shared_ptr<luvk::Device>& Device)
{
    const std::uint32_t FamilyIndex = Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value();
    return Device->GetQueue(FamilyIndex);
}

ImGuiBackendVulkan::ImGuiBackendVulkan(const VkInstance                             Instance,
                                       const std::shared_ptr<luvk::Device>&         Device,
                                       const std::shared_ptr<luvk::DescriptorPool>& Pool,
                                       const std::shared_ptr<luvk::SwapChain>&      Swap,
                                       const std::shared_ptr<luvk::Memory>&         Memory)
    : m_Instance(Instance),
      m_Device(Device),
      m_SwapChain(Swap),
      m_Memory(Memory)
{
    ImGuiIO& IO                = ImGui::GetIO();
    IO.BackendRendererName     = "ImGuiBackendVulkan_LUVK";
    IO.BackendRendererUserData = static_cast<void*>(this);
    IO.BackendFlags            |= ImGuiBackendFlags_RendererHasVtxOffset | ImGuiBackendFlags_RendererHasViewports;

    m_Mesh = std::make_shared<ImGuiMesh>(m_Device, m_Memory, Pool, Swap);

    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO& PlatformIO       = ImGui::GetPlatformIO();
        PlatformIO.Renderer_CreateWindow  = CreateWindow;
        PlatformIO.Renderer_DestroyWindow = DestroyWindow;
        PlatformIO.Renderer_SetWindowSize = SetWindowSize;
        PlatformIO.Renderer_RenderWindow  = RenderWindow;
        PlatformIO.Renderer_SwapBuffers   = SwapBuffers;
    }
}

ImGuiBackendVulkan::~ImGuiBackendVulkan()
{
    ImGuiIO& IO                = ImGui::GetIO();
    IO.BackendRendererUserData = nullptr;
    IO.BackendRendererName     = nullptr;
}

void ImGuiBackendVulkan::NewFrame() const
{
    ImGui::NewFrame();
}

void ImGuiBackendVulkan::Render(const VkCommandBuffer Cmd, const std::uint32_t CurrentFrame) const
{
    ImGui::Render();
    const ImDrawData* const DrawData = ImGui::GetDrawData();

    m_Mesh->UpdateBuffers(DrawData, CurrentFrame);
    m_Mesh->Render(Cmd, CurrentFrame);
}

void ImGuiBackendVulkan::CreateWindow(ImGuiViewport* const Viewport)
{
    const auto Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
    const auto Device  = Backend->GetDevice();

    auto* const Data           = new ViewportData();
    Viewport->RendererUserData = static_cast<void*>(Data);

    ImGui::GetPlatformIO().Platform_CreateVkSurface(Viewport,
                                                    reinterpret_cast<ImU64>(Backend->GetInstance()),
                                                    nullptr,
                                                    reinterpret_cast<ImU64*>(&Data->Surface));

    Data->CommandPool = luvk::CreateModule<luvk::CommandPool>(Device);
    Data->Sync        = luvk::CreateModule<luvk::Synchronization>(Device);
    Data->SwapChain   = luvk::CreateModule<luvk::SwapChain>(Device, Backend->GetMemory(), Data->Sync);
    Data->Draw        = luvk::CreateModule<luvk::Draw>(Device, Data->Sync);

    Data->Draw->RegisterDrawCommand(luvk::DrawCallbackInfo{[Data, Viewport](const VkCommandBuffer CommandBuffer)
    {
        if (Data && Viewport)
        {
            const std::uint32_t CurrentFrame = Data->Sync->GetCurrentFrame();
            Data->Mesh->UpdateBuffers(Viewport->DrawData, CurrentFrame);
            Data->Mesh->Render(CommandBuffer, CurrentFrame);

            return true;
        }
        return false;
    }});

    Data->Mesh = std::make_shared<ImGuiMesh>(Device,
                                             Backend->GetMemory(),
                                             Backend->GetMesh()->GetMaterial());

    Data->SwapChain->CreateSwapChain({.Extent = {.width = static_cast<std::uint32_t>(Viewport->Size.x),
                                                 .height = static_cast<std::uint32_t>(Viewport->Size.y)},
                                      .Surface = Data->Surface},
                                     nullptr);

    const std::uint32_t QueueFamilyIndex = Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value();
    Data->CommandPool->CreateCommandPool(QueueFamilyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    Data->Sync->Initialize(Data->CommandPool->AllocateRenderCommandBuffers());
}

void ImGuiBackendVulkan::DestroyWindow(ImGuiViewport* const Viewport)
{
    const auto Data = static_cast<ViewportData*>(Viewport->RendererUserData);
    if (!Data)
    {
        return;
    }

    const auto Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
    const auto Device  = Backend->GetDevice();

    Device->WaitQueue(GetGraphicsQueue(Device));

    Data->Mesh.reset();
    Data->Sync.reset();
    Data->CommandPool.reset();
    Data->SwapChain.reset();
    Data->Draw.reset();

    if (Data->Surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(Backend->GetInstance(), Data->Surface, nullptr);
    }

    delete Data;
    Viewport->RendererUserData = nullptr;
}

void ImGuiBackendVulkan::SetWindowSize(ImGuiViewport* const Viewport, const ImVec2 Size)
{
    const auto Data = static_cast<ViewportData*>(Viewport->RendererUserData);
    if (!Data)
    {
        return;
    }

    const auto Device = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData)->GetDevice();

    Device->WaitQueue(GetGraphicsQueue(Device));

    Data->SwapChain->Recreate({static_cast<uint32_t>(Size.x), static_cast<uint32_t>(Size.y)}, nullptr);
    Data->Sync->ResetFrames();
}

void ImGuiBackendVulkan::RenderWindow(ImGuiViewport* const Viewport, void*)
{
    const auto Data = static_cast<ViewportData*>(Viewport->RendererUserData);
    if (!Data)
    {
        return;
    }

    luvk::FrameData&                   Frame      = Data->Sync->GetCurrentFrameData();
    const std::optional<std::uint32_t> ImageIndex = Data->SwapChain->Acquire(Frame);

    if (!ImageIndex.has_value())
    {
        Data->ImageIndex = -1;
        return;
    }

    const std::uint32_t IndexValue = ImageIndex.value();
    Data->ImageIndex               = static_cast<std::int32_t>(IndexValue);

    Data->Draw->RecordCommands(Frame, Data->SwapChain->GetRenderTarget(IndexValue));
    Data->Draw->SubmitFrame(Frame, IndexValue);
}

void ImGuiBackendVulkan::SwapBuffers(ImGuiViewport* const Viewport, void*)
{
    const auto Data = static_cast<ViewportData*>(Viewport->RendererUserData);

    if (!Data || Data->ImageIndex < 0)
    {
        return;
    }

    Data->SwapChain->Present(Data->ImageIndex);
}
