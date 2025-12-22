// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/overlay-exp

#pragma once

#include <memory>
#include <volk.h>
#include <SDL3/SDL_events.h>
#include "Core/UserInterface/Backend/ImGuiBackendSDL.hpp"
#include "Core/UserInterface/Backend/ImGuiBackendVulkan.hpp"

class SDL_Window;

namespace luvk
{
    class Device;
    class DescriptorPool;
    class SwapChain;
    class Memory;
    class CommandBuffer;
} // namespace luvk

namespace Core
{
    class ImGuiLayerBase
    {
    protected:
        std::unique_ptr<ImGuiBackendSDL>    m_SdlBackend{};
        std::unique_ptr<ImGuiBackendVulkan> m_VulkanBackend{};

    public:
        ImGuiLayerBase() = delete;
        explicit ImGuiLayerBase(SDL_Window*                                  Window,
                                const VkInstance&                            Instance,
                                std::shared_ptr<luvk::Device> const&         Device,
                                std::shared_ptr<luvk::DescriptorPool> const& Pool,
                                std::shared_ptr<luvk::SwapChain> const&      Swap,
                                std::shared_ptr<luvk::Memory> const&         Memory);
        virtual ~ImGuiLayerBase();

        virtual void Draw();
        virtual void PushStyle() const;

        void Render(const VkCommandBuffer& Cmd, std::uint32_t CurrentFrame) const;

        [[nodiscard]] bool ProcessEvent(const SDL_Event& Event) const;
    };
} // namespace Core
