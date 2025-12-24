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
#include "Core/Meshes/ImGuiMesh.hpp"

#undef CreateWindow

using namespace Core;

// TODO : Use the same from the renderer
constexpr std::array g_ClearValues{VkClearValue{.color = {0.2F, 0.2F, 0.2F, 1.F}},
                                   VkClearValue{.depthStencil = {1.F, 0U}}};

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
    const auto  Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
    auto* const Data    = new ViewportData();

    ImGui::GetPlatformIO().Platform_CreateVkSurface(Viewport,
                                                    reinterpret_cast<ImU64>(Backend->GetInstance()),
                                                    nullptr,
                                                    reinterpret_cast<ImU64*>(&Data->Surface));

    const std::uint32_t GraphicsFamily = Backend->GetDevice()->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value();

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Backend->GetDevice()->GetPhysicalDevice(), Data->Surface, &SurfaceCapabilities);

    VkExtent2D SwapchainExtent;
    if (SurfaceCapabilities.currentExtent.width != 0xFFFFFFFF)
    {
        SwapchainExtent = SurfaceCapabilities.currentExtent;
    }
    else
    {
        SwapchainExtent.width = std::clamp(static_cast<std::uint32_t>(Viewport->Size.x),
                                           SurfaceCapabilities.minImageExtent.width,
                                           SurfaceCapabilities.maxImageExtent.width);
        SwapchainExtent.height = std::clamp(static_cast<std::uint32_t>(Viewport->Size.y),
                                            SurfaceCapabilities.minImageExtent.height,
                                            SurfaceCapabilities.maxImageExtent.height);
    }

    Data->SwapChain = std::make_shared<luvk::SwapChain>(Backend->GetDevice(), Backend->GetMemory());
    Data->SwapChain->CreateSwapChain({.Extent = SwapchainExtent, .Surface = Data->Surface}, nullptr);

    Data->CommandPool = std::make_shared<luvk::CommandPool>(Backend->GetDevice());
    Data->CommandPool->CreateCommandPool(GraphicsFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    Data->Sync = std::make_shared<luvk::Synchronization>(Backend->GetDevice(), Data->SwapChain, Data->CommandPool);
    Data->Sync->Initialize();
    Data->Sync->SetupFrames();

    Data->Mesh = std::make_shared<ImGuiMesh>(Backend->GetDevice(), Backend->GetMemory(), Backend->GetMesh()->GetMaterial());

    Viewport->RendererUserData = static_cast<void*>(Data);
}

void ImGuiBackendVulkan::DestroyWindow(ImGuiViewport* const Viewport)
{
    if (auto* Data = static_cast<ViewportData*>(Viewport->RendererUserData);
        Data != nullptr)
    {
        const auto Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
        const auto Device  = Backend->GetDevice();

        Device->Wait(Device->GetQueue(Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value()));

        Data->SwapChain.reset();
        Data->Sync.reset();
        Data->CommandPool.reset();
        Data->Mesh.reset();

        if (Data->Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData)->GetInstance(),
                                Data->Surface,
                                nullptr);
        }

        delete Data;
    }
    Viewport->RendererUserData = nullptr;
}

void ImGuiBackendVulkan::SetWindowSize(ImGuiViewport* const Viewport, const ImVec2 Size)
{
    if (const auto* Data = static_cast<ViewportData*>(Viewport->RendererUserData);
        Data != nullptr)
    {
        const auto Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
        const auto Device  = Backend->GetDevice();

        Device->Wait(Device->GetQueue(Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value()));

        Data->SwapChain->Recreate({static_cast<std::uint32_t>(Size.x), static_cast<std::uint32_t>(Size.y)}, nullptr);
    }
}

void ImGuiBackendVulkan::RenderWindow(ImGuiViewport* const Viewport, void*)
{
    const auto Backend = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData);
    const auto Data    = static_cast<ViewportData*>(Viewport->RendererUserData);
    const auto Device  = Backend->GetDevice();

    Data->Sync->AdvanceFrame();
    const auto       CurrentFrame = static_cast<std::uint32_t>(Data->Sync->GetCurrentFrame());
    luvk::FrameData& Frame        = Data->Sync->GetFrame(CurrentFrame);

    if (Frame.Submitted)
    {
        Device->Wait(Frame.InFlight, VK_TRUE, UINT64_MAX);
    }

    vkAcquireNextImageKHR(Device->GetLogicalDevice(),
                          Data->SwapChain->GetHandle(),
                          UINT64_MAX,
                          Frame.ImageAvailable,
                          VK_NULL_HANDLE,
                          &Data->ImageIndex);

    vkResetFences(Device->GetLogicalDevice(), 1U, &Frame.InFlight);
    vkResetCommandBuffer(Frame.CommandBuffer, 0U);

    constexpr VkCommandBufferBeginInfo BeginInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
    vkBeginCommandBuffer(Frame.CommandBuffer, &BeginInfo);

    const VkRenderPassBeginInfo RPBeginInfo{.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                                            .renderPass = Data->SwapChain->GetRenderPass(),
                                            .framebuffer = Data->SwapChain->GetFramebuffer(Data->ImageIndex),
                                            .renderArea = {{0, 0}, Data->SwapChain->GetExtent()},
                                            .clearValueCount = static_cast<std::uint32_t>(std::size(g_ClearValues)),
                                            .pClearValues = std::data(g_ClearValues)};

    vkCmdBeginRenderPass(Frame.CommandBuffer, &RPBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    Data->Mesh->UpdateBuffers(Viewport->DrawData, CurrentFrame);
    Data->Mesh->Render(Frame.CommandBuffer, CurrentFrame);

    vkCmdEndRenderPass(Frame.CommandBuffer);
    vkEndCommandBuffer(Frame.CommandBuffer);

    constexpr VkPipelineStageFlags WaitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    const VkSemaphore              Semaphore = Data->Sync->GetRenderFinished(Data->ImageIndex);

    const VkSubmitInfo SubmitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                  .waitSemaphoreCount = 1U,
                                  .pWaitSemaphores = &Frame.ImageAvailable,
                                  .pWaitDstStageMask = &WaitStage,
                                  .commandBufferCount = 1U,
                                  .pCommandBuffers = &Frame.CommandBuffer,
                                  .signalSemaphoreCount = 1U,
                                  .pSignalSemaphores = &Semaphore};

    vkQueueSubmit(Device->GetQueue(Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value()), 1U, &SubmitInfo, Frame.InFlight);
}

void ImGuiBackendVulkan::SwapBuffers(ImGuiViewport* const Viewport, void*)
{
    const auto Data   = static_cast<ViewportData*>(Viewport->RendererUserData);
    const auto Device = static_cast<ImGuiBackendVulkan*>(ImGui::GetIO().BackendRendererUserData)->GetDevice();

    const VkSemaphore    Semaphore = Data->Sync->GetRenderFinished(Data->ImageIndex);
    const VkSwapchainKHR SwapChain = Data->SwapChain->GetHandle();

    const VkPresentInfoKHR PresentInfo{.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                       .waitSemaphoreCount = 1U,
                                       .pWaitSemaphores = &Semaphore,
                                       .swapchainCount = 1U,
                                       .pSwapchains = &SwapChain,
                                       .pImageIndices = &Data->ImageIndex};

    vkQueuePresentKHR(Device->GetQueue(Device->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT).value()), &PresentInfo);
}
