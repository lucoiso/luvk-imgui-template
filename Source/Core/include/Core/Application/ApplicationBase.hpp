// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <memory>
#include <string>
#include <luvk/Modules/Renderer.hpp>
#include "Core/Components/InputManager.hpp"
#include "Core/UserInterface/ImGuiLayerBase.hpp"

class SDL_Window;

namespace luvk
{
    class Debug;
    class SwapChain;
    class Device;
    class Memory;
    class CommandPool;
    class Synchronization;
    class ThreadPool;
    class DescriptorPool;
    class Draw;
} // namespace luvk

namespace Core
{
    class CORE_API ApplicationBase
    {
    protected:
        bool m_CanRender{false};
        bool m_ResizePending{false};

        std::int32_t m_Width{};
        std::int32_t m_Height{};
        float        m_DeltaTime{0.f};
        std::string  m_Title{};

        SDL_Window*                     m_Window{};
        std::shared_ptr<InputManager>   m_Input;
        std::unique_ptr<ImGuiLayerBase> m_ImGuiLayer;
        std::shared_ptr<luvk::Renderer> m_Renderer{};

    public:
        explicit ApplicationBase(std::uint32_t Width, std::uint32_t Height, SDL_WindowFlags Flags);
        virtual  ~ApplicationBase() = default;

        virtual bool Initialize();
        virtual void Shutdown();

        virtual bool Render();

        template <typename ImGuiLayerType, typename... Args> requires std::is_base_of_v<Core::ImGuiLayerBase, ImGuiLayerType>
        void RegisterImGuiLayer(Args&&... InArguments)
        {
            m_ImGuiLayer = std::make_unique<ImGuiLayerType>(m_Window,
                                                            m_Renderer,
                                                            std::forward<Args>(InArguments)...);
            PostRegisterImGuiLayer();
        }

        [[nodiscard]] constexpr std::uint32_t GetWidth() const noexcept
        {
            return static_cast<std::uint32_t>(m_Width);
        }

        [[nodiscard]] constexpr std::uint32_t GetHeight() const noexcept
        {
            return static_cast<std::uint32_t>(m_Height);
        }

        void SetTitle(std::string_view Title) noexcept;

        [[nodiscard]] constexpr std::string_view GetTitle() const noexcept
        {
            return m_Title;
        }

        [[nodiscard]] constexpr float GetDeltaTime() const noexcept
        {
            return m_DeltaTime;
        }

        [[nodiscard]] std::shared_ptr<InputManager> GetInput() const noexcept
        {
            return m_Input;
        }

        [[nodiscard]] constexpr SDL_Window* GetWindow() const noexcept
        {
            return m_Window;
        }

    protected:
        virtual bool                PreRenderCallback(VkCommandBuffer CommandBuffer);
        virtual bool                PostRenderCallback(VkCommandBuffer CommandBuffer);
        virtual bool                DrawCallback(VkCommandBuffer CommandBuffer, std::uint32_t CurrentFrame);
        virtual void                UserEventCallback(const SDL_Event& Event);
        virtual void                SetupInstanceExtensions() const;
        [[nodiscard]] virtual void* GetInstanceFeatureChain() const;
        virtual void                SetupDeviceExtensions() const;
        [[nodiscard]] virtual void* GetDeviceFeatureChain() const;

    private:
        void               PostRegisterImGuiLayer();
        void               RegisterInputBindings();
        void               RegisterModules();
        [[nodiscard]] bool InitializeModules() const;
        [[nodiscard]] bool InitializeDeviceModule() const;
    };
} // namespace Core
