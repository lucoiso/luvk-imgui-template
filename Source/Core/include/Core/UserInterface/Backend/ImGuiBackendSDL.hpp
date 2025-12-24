// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <array>
#include <imgui.h>
#include <SDL3/SDL_events.h>

#undef CreateWindow

class SDL_Window;
class SDL_Cursor;
struct ImGuiViewport;

namespace Core
{
    class CORE_API ImGuiBackendSDL
    {
        SDL_Window*                 m_Window{};
        std::array<SDL_Cursor*, 11> m_MouseCursors{};

    public:
        ImGuiBackendSDL() = delete;
        explicit ImGuiBackendSDL(SDL_Window* Window);
        ~ImGuiBackendSDL();

        void               NewFrame() const;
        [[nodiscard]] bool ProcessEvent(const SDL_Event& Event) const;

    private:
        void UpdateMonitors() const;

        static void   CreateWindow(ImGuiViewport* Viewport);
        static void   DestroyWindow(ImGuiViewport* Viewport);
        static void   ShowWindow(ImGuiViewport* Viewport);
        static ImVec2 GetWindowPos(ImGuiViewport* Viewport);
        static void   SetWindowPos(ImGuiViewport* Viewport, ImVec2 Pos);
        static ImVec2 GetWindowSize(ImGuiViewport* Viewport);
        static void   SetWindowSize(ImGuiViewport* Viewport, ImVec2 Size);
        static void   SetWindowTitle(ImGuiViewport* Viewport, const char* Title);
        static void   SetWindowFocus(ImGuiViewport* Viewport);
        static bool   GetWindowFocus(ImGuiViewport* Viewport);
        static bool   GetWindowMinimized(ImGuiViewport* Viewport);
        static void   SetWindowAlpha(ImGuiViewport* Viewport, float Alpha);
        static int    CreateVkSurface(ImGuiViewport* Viewport, ImU64 Instance, const void* Allocator, ImU64* OutSurface);
    };
}
