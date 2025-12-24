// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Components/InputManager.hpp"
#include <SDL3/SDL_mouse.h>

using namespace Core;

InputManager::InputManager(SDL_Window* MainWindow)
    : m_MainWindow(MainWindow) {}

void InputManager::BindEvent(const std::uint32_t                     Type,
                             std::function<void(const SDL_Event&)>&& Callback)
{
    auto [Iterator, Inserted] = m_Bindings.try_emplace(Type, EventCallbacks{});
    Iterator->second.emplace_back(std::move(Callback));
}

void InputManager::ProcessEvents()
{
    SDL_Event Event;

    while (SDL_PollEvent(&Event))
    {
        switch (Event.type)
        {
        case SDL_EVENT_QUIT:
            {
                m_Running = false;
                break;
            }
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                if (Event.window.windowID == SDL_GetWindowID(m_MainWindow))
                {
                    m_Running = false;
                }
                break;
            }
        case SDL_EVENT_KEY_DOWN:
            {
                m_PressedKeys.insert(Event.key.key);
                break;
            }
        case SDL_EVENT_KEY_UP:
            {
                m_PressedKeys.erase(Event.key.key);
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                {
                    m_LeftHeld = true;
                }
                else if (Event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_RightHeld = false;
                }
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                {
                    m_LeftHeld = false;
                }
                else if (Event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_RightHeld = false;
                }
                break;
            }
        default: break;
        }

        InvokeCallbacks(Event.type, Event);
        InvokeCallbacks(SDL_EVENT_USER, Event);
    }
}

void InputManager::InvokeCallbacks(const std::uint32_t Type, const SDL_Event& Event)
{
    if (const auto It = m_Bindings.find(Type);
        It != std::end(m_Bindings))
    {
        for (const auto& CallbackIt : It->second)
        {
            CallbackIt(Event);
        }
    }
}

bool InputManager::IsKeyPressed(const std::uint32_t key) const noexcept
{
    return m_PressedKeys.contains(key);
}
