// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <imgui.h>
#include <memory>
#include <volk.h>

namespace luvk
{
    class DescriptorPool;
    class Device;
    class Memory;
    class SwapChain;
    class CommandPool;
    class Synchronization;
    class DescriptorSet;
    class Pipeline;
    class Mesh;
    class Texture;
}

namespace Core
{
    class ImGuiMesh;

    struct CORE_API ViewportData
    {
        std::uint32_t                          ImageIndex{0U};
        VkSurfaceKHR                           Surface{VK_NULL_HANDLE};
        std::shared_ptr<luvk::SwapChain>       SwapChain{nullptr};
        std::shared_ptr<luvk::CommandPool>     CommandPool{nullptr};
        std::shared_ptr<luvk::Synchronization> Sync{nullptr};
        std::shared_ptr<ImGuiMesh>             Mesh{nullptr};
    };

    class CORE_API ImGuiBackendVulkan
    {
        VkInstance                       m_Instance{VK_NULL_HANDLE};
        std::shared_ptr<luvk::Device>    m_Device{};
        std::shared_ptr<luvk::SwapChain> m_SwapChain{};
        std::shared_ptr<luvk::Memory>    m_Memory{};
        std::shared_ptr<ImGuiMesh>       m_Mesh{};

    public:
        ImGuiBackendVulkan() = delete;
        explicit ImGuiBackendVulkan(const VkInstance                             Instance,
                                    const std::shared_ptr<luvk::Device>&         Device,
                                    const std::shared_ptr<luvk::DescriptorPool>& Pool,
                                    const std::shared_ptr<luvk::SwapChain>&      Swap,
                                    const std::shared_ptr<luvk::Memory>&         Memory);
        ~ImGuiBackendVulkan();

        void NewFrame() const;
        void Render(VkCommandBuffer Cmd, std::uint32_t CurrentFrame) const;

        [[nodiscard]] VkInstance GetInstance() const
        {
            return m_Instance;
        }

        [[nodiscard]] std::shared_ptr<ImGuiMesh> GetMesh() const noexcept
        {
            return m_Mesh;
        }

        [[nodiscard]] std::shared_ptr<luvk::Device> GetDevice() const noexcept
        {
            return m_Device;
        }

        [[nodiscard]] std::shared_ptr<luvk::Memory> GetMemory() const noexcept
        {
            return m_Memory;
        }

    private:
        static void CreateWindow(ImGuiViewport* Viewport);
        static void DestroyWindow(ImGuiViewport* Viewport);
        static void SetWindowSize(ImGuiViewport* Viewport, ImVec2 Size);
        static void RenderWindow(ImGuiViewport* Viewport, void* RenderArg);
        static void SwapBuffers(ImGuiViewport* Viewport, void* RenderArg);
    };
}
