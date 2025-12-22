// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/overlay-exp

#pragma once

#include <memory>
#include <string>
#include "Core/Components/InputManager.hpp"

class SDL_Window;

namespace luvk
{
    class Renderer;
    class Debug;
    class SwapChain;
    class Device;
    class Memory;
    class CommandPool;
    class Synchronization;
    class ThreadPool;
    class DescriptorPool;
} // namespace luvk

namespace Core
{
    class ApplicationBase
    {
    protected:
        bool m_CanRender{false};
        bool m_ResizePending{false};

        std::int32_t m_Width{};
        std::int32_t m_Height{};
        std::string  m_Title{};

        float m_DeltaTime{0.f};

        SDL_Window*                   m_Window{};
        std::shared_ptr<InputManager> m_Input;

        std::shared_ptr<luvk::Renderer>        m_Renderer{};
        std::shared_ptr<luvk::Debug>           m_DebugModule{};
        std::shared_ptr<luvk::SwapChain>       m_SwapChainModule{};
        std::shared_ptr<luvk::Device>          m_DeviceModule{};
        std::shared_ptr<luvk::Memory>          m_MemoryModule{};
        std::shared_ptr<luvk::CommandPool>     m_CommandPoolModule{};
        std::shared_ptr<luvk::Synchronization> m_SynchronizationModule{};
        std::shared_ptr<luvk::ThreadPool>      m_ThreadPoolModule{};
        std::shared_ptr<luvk::DescriptorPool>  m_DescriptorPoolModule{};

    public:
        explicit ApplicationBase(std::uint32_t Width, std::uint32_t Height, SDL_WindowFlags Flags);
        virtual  ~ApplicationBase();

        virtual bool Initialize();
        virtual bool Render();

        [[nodiscard]] constexpr std::uint32_t GetWidth() const noexcept
        {
            return static_cast<std::uint32_t>(m_Width);
        }

        [[nodiscard]] constexpr std::uint32_t GetHeight() const noexcept
        {
            return static_cast<std::uint32_t>(m_Height);
        }

        void SetTitle(std::string_view Title);

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

    private:
        void               RegisterModules();
        void               SetupExtensions() const;
        [[nodiscard]] bool InitializeModules() const;
        [[nodiscard]] bool InitializeDeviceModule() const;
    };
} // namespace Core
