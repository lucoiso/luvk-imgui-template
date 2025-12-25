// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/UserInterface/Backend/ImGuiBackendSDL.hpp"
#include <imgui.h>
#include <volk.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>

#undef CreateWindow

using namespace Core;

static ImGuiKey SDLKeyToImGuiKey(const std::uint32_t Keycode, const SDL_Scancode Scancode)
{
    switch (Scancode)
    {
    case SDL_SCANCODE_KP_0: return ImGuiKey_Keypad0;
    case SDL_SCANCODE_KP_1: return ImGuiKey_Keypad1;
    case SDL_SCANCODE_KP_2: return ImGuiKey_Keypad2;
    case SDL_SCANCODE_KP_3: return ImGuiKey_Keypad3;
    case SDL_SCANCODE_KP_4: return ImGuiKey_Keypad4;
    case SDL_SCANCODE_KP_5: return ImGuiKey_Keypad5;
    case SDL_SCANCODE_KP_6: return ImGuiKey_Keypad6;
    case SDL_SCANCODE_KP_7: return ImGuiKey_Keypad7;
    case SDL_SCANCODE_KP_8: return ImGuiKey_Keypad8;
    case SDL_SCANCODE_KP_9: return ImGuiKey_Keypad9;
    case SDL_SCANCODE_KP_PERIOD: return ImGuiKey_KeypadDecimal;
    case SDL_SCANCODE_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case SDL_SCANCODE_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case SDL_SCANCODE_KP_MINUS: return ImGuiKey_KeypadSubtract;
    case SDL_SCANCODE_KP_PLUS: return ImGuiKey_KeypadAdd;
    case SDL_SCANCODE_KP_ENTER: return ImGuiKey_KeypadEnter;
    case SDL_SCANCODE_KP_EQUALS: return ImGuiKey_KeypadEqual;
    default:
        {
            break;
        }
    }

    switch (Keycode)
    {
    case SDLK_TAB: return ImGuiKey_Tab;
    case SDLK_LEFT: return ImGuiKey_LeftArrow;
    case SDLK_RIGHT: return ImGuiKey_RightArrow;
    case SDLK_UP: return ImGuiKey_UpArrow;
    case SDLK_DOWN: return ImGuiKey_DownArrow;
    case SDLK_PAGEUP: return ImGuiKey_PageUp;
    case SDLK_PAGEDOWN: return ImGuiKey_PageDown;
    case SDLK_HOME: return ImGuiKey_Home;
    case SDLK_END: return ImGuiKey_End;
    case SDLK_INSERT: return ImGuiKey_Insert;
    case SDLK_DELETE: return ImGuiKey_Delete;
    case SDLK_BACKSPACE: return ImGuiKey_Backspace;
    case SDLK_SPACE: return ImGuiKey_Space;
    case SDLK_RETURN: return ImGuiKey_Enter;
    case SDLK_ESCAPE: return ImGuiKey_Escape;
    case SDLK_APOSTROPHE: return ImGuiKey_Apostrophe;
    case SDLK_COMMA: return ImGuiKey_Comma;
    case SDLK_MINUS: return ImGuiKey_Minus;
    case SDLK_PERIOD: return ImGuiKey_Period;
    case SDLK_SLASH: return ImGuiKey_Slash;
    case SDLK_SEMICOLON: return ImGuiKey_Semicolon;
    case SDLK_EQUALS: return ImGuiKey_Equal;
    case SDLK_LEFTBRACKET: return ImGuiKey_LeftBracket;
    case SDLK_BACKSLASH: return ImGuiKey_Backslash;
    case SDLK_RIGHTBRACKET: return ImGuiKey_RightBracket;
    case SDLK_GRAVE: return ImGuiKey_GraveAccent;
    case SDLK_CAPSLOCK: return ImGuiKey_CapsLock;
    case SDLK_SCROLLLOCK: return ImGuiKey_ScrollLock;
    case SDLK_NUMLOCKCLEAR: return ImGuiKey_NumLock;
    case SDLK_PRINTSCREEN: return ImGuiKey_PrintScreen;
    case SDLK_PAUSE: return ImGuiKey_Pause;
    case SDLK_LCTRL: return ImGuiKey_LeftCtrl;
    case SDLK_LSHIFT: return ImGuiKey_LeftShift;
    case SDLK_LALT: return ImGuiKey_LeftAlt;
    case SDLK_LGUI: return ImGuiKey_LeftSuper;
    case SDLK_RCTRL: return ImGuiKey_RightCtrl;
    case SDLK_RSHIFT: return ImGuiKey_RightShift;
    case SDLK_RALT: return ImGuiKey_RightAlt;
    case SDLK_RGUI: return ImGuiKey_RightSuper;
    case SDLK_APPLICATION: return ImGuiKey_Menu;
    case SDLK_0: return ImGuiKey_0;
    case SDLK_1: return ImGuiKey_1;
    case SDLK_2: return ImGuiKey_2;
    case SDLK_3: return ImGuiKey_3;
    case SDLK_4: return ImGuiKey_4;
    case SDLK_5: return ImGuiKey_5;
    case SDLK_6: return ImGuiKey_6;
    case SDLK_7: return ImGuiKey_7;
    case SDLK_8: return ImGuiKey_8;
    case SDLK_9: return ImGuiKey_9;
    case SDLK_A: return ImGuiKey_A;
    case SDLK_B: return ImGuiKey_B;
    case SDLK_C: return ImGuiKey_C;
    case SDLK_D: return ImGuiKey_D;
    case SDLK_E: return ImGuiKey_E;
    case SDLK_F: return ImGuiKey_F;
    case SDLK_G: return ImGuiKey_G;
    case SDLK_H: return ImGuiKey_H;
    case SDLK_I: return ImGuiKey_I;
    case SDLK_J: return ImGuiKey_J;
    case SDLK_K: return ImGuiKey_K;
    case SDLK_L: return ImGuiKey_L;
    case SDLK_M: return ImGuiKey_M;
    case SDLK_N: return ImGuiKey_N;
    case SDLK_O: return ImGuiKey_O;
    case SDLK_P: return ImGuiKey_P;
    case SDLK_Q: return ImGuiKey_Q;
    case SDLK_R: return ImGuiKey_R;
    case SDLK_S: return ImGuiKey_S;
    case SDLK_T: return ImGuiKey_T;
    case SDLK_U: return ImGuiKey_U;
    case SDLK_V: return ImGuiKey_V;
    case SDLK_W: return ImGuiKey_W;
    case SDLK_X: return ImGuiKey_X;
    case SDLK_Y: return ImGuiKey_Y;
    case SDLK_Z: return ImGuiKey_Z;
    default:
        {
            break;
        }
    }

    switch (Scancode)
    {
    case SDL_SCANCODE_GRAVE: return ImGuiKey_GraveAccent;
    case SDL_SCANCODE_MINUS: return ImGuiKey_Minus;
    case SDL_SCANCODE_EQUALS: return ImGuiKey_Equal;
    case SDL_SCANCODE_LEFTBRACKET: return ImGuiKey_LeftBracket;
    case SDL_SCANCODE_RIGHTBRACKET: return ImGuiKey_RightBracket;
    case SDL_SCANCODE_NONUSBACKSLASH: return ImGuiKey_Oem102;
    case SDL_SCANCODE_BACKSLASH: return ImGuiKey_Backslash;
    case SDL_SCANCODE_SEMICOLON: return ImGuiKey_Semicolon;
    case SDL_SCANCODE_APOSTROPHE: return ImGuiKey_Apostrophe;
    case SDL_SCANCODE_COMMA: return ImGuiKey_Comma;
    case SDL_SCANCODE_PERIOD: return ImGuiKey_Period;
    case SDL_SCANCODE_SLASH: return ImGuiKey_Slash;
    default:
        {
            break;
        }
    }

    return ImGuiKey_None;
}

ImGuiBackendSDL::ImGuiBackendSDL(SDL_Window* Window)
    : m_Window(Window)
{
    ImGuiIO& IO                = ImGui::GetIO();
    IO.BackendPlatformName     = "ImGuiBackendSDL_LUVK";
    IO.BackendPlatformUserData = static_cast<void*>(this);
    IO.BackendFlags            |= ImGuiBackendFlags_HasMouseCursors;
    IO.BackendFlags            |= ImGuiBackendFlags_HasSetMousePos;
    IO.BackendFlags            |= ImGuiBackendFlags_PlatformHasViewports;
    IO.BackendFlags            |= ImGuiBackendFlags_HasMouseHoveredViewport;

    m_MouseCursors.at(ImGuiMouseCursor_Arrow)      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    m_MouseCursors.at(ImGuiMouseCursor_TextInput)  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    m_MouseCursors.at(ImGuiMouseCursor_ResizeAll)  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_MOVE);
    m_MouseCursors.at(ImGuiMouseCursor_ResizeNS)   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NS_RESIZE);
    m_MouseCursors.at(ImGuiMouseCursor_ResizeEW)   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_EW_RESIZE);
    m_MouseCursors.at(ImGuiMouseCursor_ResizeNESW) = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NESW_RESIZE);
    m_MouseCursors.at(ImGuiMouseCursor_ResizeNWSE) = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE);
    m_MouseCursors.at(ImGuiMouseCursor_Hand)       = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    m_MouseCursors.at(ImGuiMouseCursor_NotAllowed) = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NOT_ALLOWED);

    ImGuiViewport* MainViewport  = ImGui::GetMainViewport();
    MainViewport->PlatformHandle = static_cast<void*>(m_Window);

    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

        ImGuiPlatformIO& PlatformIO            = ImGui::GetPlatformIO();
        PlatformIO.Platform_CreateWindow       = CreateWindow;
        PlatformIO.Platform_DestroyWindow      = DestroyWindow;
        PlatformIO.Platform_ShowWindow         = ShowWindow;
        PlatformIO.Platform_GetWindowPos       = GetWindowPos;
        PlatformIO.Platform_SetWindowPos       = SetWindowPos;
        PlatformIO.Platform_GetWindowSize      = GetWindowSize;
        PlatformIO.Platform_SetWindowSize      = SetWindowSize;
        PlatformIO.Platform_SetWindowTitle     = SetWindowTitle;
        PlatformIO.Platform_SetWindowFocus     = SetWindowFocus;
        PlatformIO.Platform_GetWindowFocus     = GetWindowFocus;
        PlatformIO.Platform_GetWindowMinimized = GetWindowMinimized;
        PlatformIO.Platform_SetWindowAlpha     = SetWindowAlpha;
        PlatformIO.Platform_CreateVkSurface    = CreateVkSurface;

        UpdateMonitors();
    }
}

ImGuiBackendSDL::~ImGuiBackendSDL()
{
    for (SDL_Cursor*& Cursor : m_MouseCursors)
    {
        if (Cursor != nullptr)
        {
            SDL_DestroyCursor(Cursor);
            Cursor = nullptr;
        }
    }

    ImGui::DestroyPlatformWindows();

    ImGuiIO& IO                = ImGui::GetIO();
    IO.BackendPlatformUserData = nullptr;
    IO.BackendPlatformName     = nullptr;
}

void ImGuiBackendSDL::NewFrame() const
{
    ImGuiIO& IO = ImGui::GetIO();

    std::int32_t Width  = 0;
    std::int32_t Height = 0;
    SDL_GetWindowSize(m_Window, &Width, &Height);

    std::int32_t DisplayWidth  = 0;
    std::int32_t DisplayHeight = 0;
    SDL_GetWindowSizeInPixels(m_Window, &DisplayWidth, &DisplayHeight);

    IO.DisplaySize = ImVec2(static_cast<float>(Width), static_cast<float>(Height));

    if (Width > 0 && Height > 0)
    {
        IO.DisplayFramebufferScale = ImVec2(static_cast<float>(DisplayWidth) / static_cast<float>(Width),
                                            static_cast<float>(DisplayHeight) / static_cast<float>(Height));
    }

    if (IO.WantSetMousePos)
    {
        SDL_WarpMouseInWindow(m_Window, IO.MousePos.x, IO.MousePos.y);
    }

    if (!(IO.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
    {
        if (const ImGuiMouseCursor CursorType = ImGui::GetMouseCursor();
            IO.MouseDrawCursor || CursorType == ImGuiMouseCursor_None)
        {
            SDL_HideCursor();
        }
        else
        {
            SDL_Cursor* const NewCursor = m_MouseCursors.at(CursorType)
                                              ? m_MouseCursors.at(CursorType)
                                              : m_MouseCursors.at(ImGuiMouseCursor_Arrow);

            SDL_SetCursor(NewCursor);
            SDL_ShowCursor();
        }
    }
}

bool ImGuiBackendSDL::ProcessEvent(const SDL_Event& Event) const
{
    ImGuiIO& IO = ImGui::GetIO();

    switch (Event.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        {
            ImVec2 MousePos(Event.motion.x, Event.motion.y);

            if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                std::int32_t WindowX = 0;
                std::int32_t WindowY = 0;
                SDL_GetWindowPosition(SDL_GetWindowFromID(Event.motion.windowID), &WindowX, &WindowY);
                MousePos.x += static_cast<float>(WindowX);
                MousePos.y += static_cast<float>(WindowY);
            }

            IO.AddMousePosEvent(MousePos.x, MousePos.y);

            return IO.WantCaptureMouse;
        }
    case SDL_EVENT_MOUSE_WHEEL:
        {
            IO.AddMouseWheelEvent(Event.wheel.x, Event.wheel.y);
            return IO.WantCaptureMouse;
        }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            std::int32_t Button = -1;
            if (Event.button.button == SDL_BUTTON_LEFT)
            {
                Button = 0;
            }
            if (Event.button.button == SDL_BUTTON_RIGHT)
            {
                Button = 1;
            }
            if (Event.button.button == SDL_BUTTON_MIDDLE)
            {
                Button = 2;
            }

            if (Button != -1)
            {
                if (const SDL_Window* const Window = SDL_GetWindowFromID(Event.button.windowID))
                {
                    if (const ImGuiViewport* const Viewport = ImGui::FindViewportByPlatformHandle(const_cast<SDL_Window*>(Window)))
                    {
                        IO.AddMouseViewportEvent(Viewport->ID);

                        if (Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !(SDL_GetWindowFlags(const_cast<SDL_Window*>(Window)) & SDL_WINDOW_INPUT_FOCUS))
                        {
                            SDL_RaiseWindow(const_cast<SDL_Window*>(Window));
                        }
                    }
                }

                IO.AddMouseButtonEvent(Button, Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
            }
            return IO.WantCaptureMouse;
        }
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        {
            if (const SDL_Window* const Window = SDL_GetWindowFromID(Event.window.windowID))
            {
                if (ImGuiViewport* const Viewport = ImGui::FindViewportByPlatformHandle(const_cast<SDL_Window*>(Window)))
                {
                    if (Viewport->PlatformHandle == static_cast<void*>(m_Window))
                    {
                        return false;
                    }

                    Viewport->PlatformRequestClose = true;
                }
            }
            return true;
        }
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        {
            if (const SDL_Window* const Window = SDL_GetWindowFromID(Event.window.windowID))
            {
                if (const ImGuiViewport* const Viewport = ImGui::FindViewportByPlatformHandle(const_cast<SDL_Window*>(Window)))
                {
                    IO.AddMouseViewportEvent(Event.type == SDL_EVENT_WINDOW_MOUSE_ENTER
                                                 ? Viewport->ID
                                                 : 0);
                }
            }
            return true;
        }
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        {
            IO.AddFocusEvent(Event.type == SDL_EVENT_WINDOW_FOCUS_GAINED);
            return true;
        }
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        {
            IO.AddKeyEvent(ImGuiMod_Ctrl, (Event.key.mod & SDL_KMOD_CTRL) != 0);
            IO.AddKeyEvent(ImGuiMod_Shift, (Event.key.mod & SDL_KMOD_SHIFT) != 0);
            IO.AddKeyEvent(ImGuiMod_Alt, (Event.key.mod & SDL_KMOD_ALT) != 0);
            IO.AddKeyEvent(ImGuiMod_Super, (Event.key.mod & SDL_KMOD_GUI) != 0);

            const ImGuiKey Key = SDLKeyToImGuiKey(Event.key.key, Event.key.scancode);
            IO.AddKeyEvent(Key, Event.type == SDL_EVENT_KEY_DOWN);

            return IO.WantCaptureKeyboard;
        }
    case SDL_EVENT_DISPLAY_ORIENTATION:
    case SDL_EVENT_DISPLAY_ADDED:
    case SDL_EVENT_DISPLAY_REMOVED:
    case SDL_EVENT_DISPLAY_MOVED:
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        {
            UpdateMonitors();
            return true;
        }
    default:
        {
            break;
        }
    }
    return false;
}

void ImGuiBackendSDL::UpdateMonitors() const
{
    ImGuiPlatformIO& PlatformIO = ImGui::GetPlatformIO();
    PlatformIO.Monitors.resize(0);

    std::int32_t DisplayCount = 0;

    if (const SDL_DisplayID* const Displays = SDL_GetDisplays(&DisplayCount))
    {
        for (std::int32_t DisplayIt = 0; DisplayIt < DisplayCount; ++DisplayIt)
        {
            ImGuiPlatformMonitor Monitor;
            SDL_Rect             Bounds;
            SDL_Rect             UsableBounds;

            SDL_GetDisplayBounds(Displays[DisplayIt], &Bounds);
            SDL_GetDisplayUsableBounds(Displays[DisplayIt], &UsableBounds);

            Monitor.MainPos  = ImVec2(static_cast<float>(Bounds.x), static_cast<float>(Bounds.y));
            Monitor.MainSize = ImVec2(static_cast<float>(Bounds.w), static_cast<float>(Bounds.h));
            Monitor.WorkPos  = ImVec2(static_cast<float>(UsableBounds.x), static_cast<float>(UsableBounds.y));
            Monitor.WorkSize = ImVec2(static_cast<float>(UsableBounds.w), static_cast<float>(UsableBounds.h));
            Monitor.DpiScale = SDL_GetDisplayContentScale(Displays[DisplayIt]);

            PlatformIO.Monitors.push_back(Monitor);
        }

        SDL_free(const_cast<SDL_DisplayID*>(Displays));
    }
}

void ImGuiBackendSDL::CreateWindow(ImGuiViewport* const Viewport)
{
    SDL_WindowFlags WindowFlags = SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN;

    if (Viewport->Flags & ImGuiViewportFlags_NoDecoration)
    {
        WindowFlags |= SDL_WINDOW_BORDERLESS;
    }
    if (Viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
    {
        WindowFlags |= SDL_WINDOW_UTILITY;
    }
    if (Viewport->Flags & ImGuiViewportFlags_TopMost)
    {
        WindowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }

    Viewport->PlatformHandle = static_cast<void*>(SDL_CreateWindow("No Title Yet",
                                                                   static_cast<int>(Viewport->Size.x),
                                                                   static_cast<int>(Viewport->Size.y),
                                                                   WindowFlags));

    SDL_SetWindowPosition(static_cast<SDL_Window*>(Viewport->PlatformHandle),
                          static_cast<int>(Viewport->Pos.x),
                          static_cast<int>(Viewport->Pos.y));
}

void ImGuiBackendSDL::DestroyWindow(ImGuiViewport* const Viewport)
{
    if (Viewport->PlatformHandle != nullptr)
    {
        SDL_DestroyWindow(static_cast<SDL_Window*>(Viewport->PlatformHandle));
    }
    Viewport->PlatformHandle = nullptr;
}

void ImGuiBackendSDL::ShowWindow(ImGuiViewport* const Viewport)
{
    SDL_ShowWindow(static_cast<SDL_Window*>(Viewport->PlatformHandle));
}

ImVec2 ImGuiBackendSDL::GetWindowPos(ImGuiViewport* const Viewport)
{
    std::int32_t PosX = 0;
    std::int32_t PosY = 0;
    SDL_GetWindowPosition(static_cast<SDL_Window*>(Viewport->PlatformHandle), &PosX, &PosY);
    return {static_cast<float>(PosX), static_cast<float>(PosY)};
}

void ImGuiBackendSDL::SetWindowPos(ImGuiViewport* const Viewport, const ImVec2 Pos)
{
    SDL_SetWindowPosition(static_cast<SDL_Window*>(Viewport->PlatformHandle),
                          static_cast<int>(Pos.x),
                          static_cast<int>(Pos.y));
}

ImVec2 ImGuiBackendSDL::GetWindowSize(ImGuiViewport* const Viewport)
{
    std::int32_t Width  = 0;
    std::int32_t Height = 0;
    SDL_GetWindowSize(static_cast<SDL_Window*>(Viewport->PlatformHandle), &Width, &Height);
    return {static_cast<float>(Width), static_cast<float>(Height)};
}

void ImGuiBackendSDL::SetWindowSize(ImGuiViewport* const Viewport, const ImVec2 Size)
{
    SDL_SetWindowSize(static_cast<SDL_Window*>(Viewport->PlatformHandle),
                      static_cast<int>(Size.x),
                      static_cast<int>(Size.y));
}

void ImGuiBackendSDL::SetWindowTitle(ImGuiViewport* const Viewport, const char* Title)
{
    SDL_SetWindowTitle(static_cast<SDL_Window*>(Viewport->PlatformHandle), Title);
}

void ImGuiBackendSDL::SetWindowFocus(ImGuiViewport* const Viewport)
{
    SDL_RaiseWindow(static_cast<SDL_Window*>(Viewport->PlatformHandle));
}

bool ImGuiBackendSDL::GetWindowFocus(ImGuiViewport* const Viewport)
{
    return (SDL_GetWindowFlags(static_cast<SDL_Window*>(Viewport->PlatformHandle)) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

bool ImGuiBackendSDL::GetWindowMinimized(ImGuiViewport* const Viewport)
{
    return (SDL_GetWindowFlags(static_cast<SDL_Window*>(Viewport->PlatformHandle)) & SDL_WINDOW_MINIMIZED) != 0;
}

void ImGuiBackendSDL::SetWindowAlpha(ImGuiViewport* const Viewport, const float Alpha)
{
    SDL_SetWindowOpacity(static_cast<SDL_Window*>(Viewport->PlatformHandle), Alpha);
}

int ImGuiBackendSDL::CreateVkSurface(ImGuiViewport* const Viewport, const ImU64 Instance, const void* Allocator, ImU64* OutSurface)
{
    const auto WindowHandle        = static_cast<SDL_Window*>(Viewport->PlatformHandle);
    const auto InstanceHandle      = reinterpret_cast<VkInstance>(Instance);
    const auto AllocationCallbacks = static_cast<const VkAllocationCallbacks*>(Allocator);
    const auto SurfaceHandle       = reinterpret_cast<VkSurfaceKHR*>(OutSurface);

    return SDL_Vulkan_CreateSurface(WindowHandle,
                                    InstanceHandle,
                                    AllocationCallbacks,
                                    SurfaceHandle);
}
