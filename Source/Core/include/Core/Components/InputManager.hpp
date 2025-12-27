/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <SDL3/SDL_events.h>

namespace Core
{
    class InputManager;

    class CORE_API EventHandle
    {
        friend class InputManager;
        std::weak_ptr<InputManager> m_Manager;
        std::uint32_t               m_Type{0};
        std::size_t                 m_Id{0};

        EventHandle(std::weak_ptr<InputManager> Manager, std::uint32_t Type, std::size_t Id);

    public:
        EventHandle() = default;
        ~EventHandle();

        EventHandle(const EventHandle&)            = delete;
        EventHandle& operator=(const EventHandle&) = delete;

        EventHandle(EventHandle&& Other) noexcept;
        EventHandle& operator=(EventHandle&& Other) noexcept;

        void Unbind();
    };

    class CORE_API InputManager : public std::enable_shared_from_this<InputManager>
    {
        struct CallbackData
        {
            std::size_t                                        Id;
            std::function<void(const SDL_Event&, SDL_Window*)> Function;
        };

        using EventCallbacks = std::vector<CallbackData>;

        bool m_Running{true};
        bool m_LeftHeld{false};
        bool m_RightHeld{false};

        SDL_Window*                                       m_MainWindow{nullptr};
        std::uint32_t                                     m_WindowID{0};
        std::unordered_map<std::uint32_t, EventCallbacks> m_Bindings{};
        std::unordered_set<std::uint32_t>                 m_PressedKeys{};
        std::size_t                                       m_NextCallbackId{0};

    public:
        InputManager() = delete;
        explicit InputManager(SDL_Window* MainWindow);
        ~InputManager() = default;

        [[nodiscard]] EventHandle BindEvent(std::uint32_t Type, std::function<void(const SDL_Event&, SDL_Window*)>&& Callback);
        void                      UnbindEvent(std::uint32_t Type, std::size_t Id);
        void                      ProcessEvents();

        [[nodiscard]] constexpr bool Running() const noexcept
        {
            return m_Running;
        }

        [[nodiscard]] bool IsKeyPressed(std::uint32_t Key) const noexcept;

        [[nodiscard]] constexpr bool LeftHeld() const noexcept
        {
            return m_LeftHeld;
        }

        [[nodiscard]] constexpr bool RightHeld() const noexcept
        {
            return m_RightHeld;
        }

    private:
        void InvokeCallbacks(std::uint32_t Type, const SDL_Event& Event);
    };
}
