// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <functional>
#include <unordered_set>
#include <SDL3/SDL_events.h>

namespace Core
{
    class CORE_API InputManager
    {
        using EventCallbacks = std::vector<std::function<void(const SDL_Event&)>>;

        bool m_Running{true};
        bool m_LeftHeld{false};
        bool m_RightHeld{false};

        SDL_Window*                                       m_MainWindow{nullptr};
        std::unordered_map<std::uint32_t, EventCallbacks> m_Bindings{};
        std::unordered_set<std::uint32_t>                 m_PressedKeys{};

    public:
        InputManager() = delete;
        explicit InputManager(SDL_Window* MainWindow);
        ~InputManager() = default;

        void BindEvent(std::uint32_t Type, std::function<void(const SDL_Event&)>&& Callback);
        void ProcessEvents();

        [[nodiscard]] constexpr bool Running() const noexcept
        {
            return m_Running;
        }

        [[nodiscard]] bool IsKeyPressed(std::uint32_t key) const noexcept;

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
} // namespace Core
