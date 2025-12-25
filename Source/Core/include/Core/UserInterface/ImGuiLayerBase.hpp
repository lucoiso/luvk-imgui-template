// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <memory>
#include <volk.h>
#include <SDL3/SDL_events.h>
#include "Core/UserInterface/Backend/ImGuiBackendSDL.hpp"
#include "Core/UserInterface/Backend/ImGuiBackendVulkan.hpp"

class SDL_Window;

namespace luvk
{
    class Renderer;
} // namespace luvk

namespace Core
{
    class CORE_API ImGuiLayerBase
    {
    protected:
        std::unique_ptr<ImGuiBackendSDL>    m_SdlBackend{};
        std::unique_ptr<ImGuiBackendVulkan> m_VulkanBackend{};

    public:
        ImGuiLayerBase() = delete;
        explicit ImGuiLayerBase(SDL_Window*                            Window,
                                std::shared_ptr<luvk::Renderer> const& Renderer);
        virtual ~ImGuiLayerBase();

        virtual void Draw();
        virtual void PushStyle() const;

        void Render(VkCommandBuffer Cmd, std::uint32_t CurrentFrame) const;

        [[nodiscard]] bool ProcessEvent(const SDL_Event& Event) const;
    };
} // namespace Core
