/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#include "Core/Components/InputManager.hpp"
#include <algorithm>
#include <SDL3/SDL_mouse.h>

using namespace Core;

EventHandle::EventHandle(std::weak_ptr<InputManager> Manager, const std::uint32_t Type, const std::size_t Id)
    : m_Manager(std::move(Manager)),
      m_Type(Type),
      m_Id(Id) {}

EventHandle::~EventHandle()
{
    Unbind();
}

EventHandle::EventHandle(EventHandle&& Other) noexcept
    : m_Manager(std::move(Other.m_Manager)),
      m_Type(Other.m_Type),
      m_Id(Other.m_Id)
{
    Other.m_Id = 0;
}

EventHandle& EventHandle::operator=(EventHandle&& Other) noexcept
{
    if (this != &Other)
    {
        Unbind();
        m_Manager  = std::move(Other.m_Manager);
        m_Type     = Other.m_Type;
        m_Id       = Other.m_Id;
        Other.m_Id = 0;
    }
    return *this;
}

void EventHandle::Unbind()
{
    if (m_Id != 0)
    {
        if (const auto Ptr = m_Manager.lock())
        {
            Ptr->UnbindEvent(m_Type, m_Id);
        }
        m_Id = 0;
    }
}

InputManager::InputManager(SDL_Window* MainWindow)
    : m_MainWindow(MainWindow),
      m_WindowID(SDL_GetWindowID(MainWindow)) {}

EventHandle InputManager::BindEvent(const std::uint32_t Type, std::function<void(const SDL_Event&, SDL_Window*)>&& Callback)
{
    auto              [Iterator, Inserted] = m_Bindings.try_emplace(Type, EventCallbacks{});
    const std::size_t Id                   = ++m_NextCallbackId;
    Iterator->second.emplace_back(CallbackData{Id,
                                               std::move(Callback)});

    return { shared_from_this(), Type, Id };
}

void InputManager::UnbindEvent(const std::uint32_t Type, const std::size_t Id)
{
    if (const auto It = m_Bindings.find(Type);
        It != std::end(m_Bindings))
    {
        std::erase_if(It->second,
                      [Id](const CallbackData& Data)
                      {
                          return Data.Id == Id;
                      });
    }
}

void InputManager::ProcessEvents()
{
    SDL_Event Event;

    while (SDL_PollEvent(&Event))
    {
        if (Event.type >= SDL_EVENT_WINDOW_FIRST && Event.type <= SDL_EVENT_WINDOW_LAST)
        {
            if (Event.window.windowID != m_WindowID)
            {
                continue;
            }
        }

        switch (Event.type)
        {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                m_Running = false;
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
                    m_RightHeld = true;
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
            CallbackIt.Function(Event, m_MainWindow);
        }
    }
}

bool InputManager::IsKeyPressed(const std::uint32_t Key) const noexcept
{
    return m_PressedKeys.contains(Key);
}
