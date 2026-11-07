// Author: Lucas Vilas-Boas
// Year: 2026
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <functional>
#include <imgui.h>

namespace luvk::platform
{
    class Window;
}

namespace Template
{
    /** Small borderless neon window with an animated HELLO WORLD title, custom chrome and a close button. */
    class HelloWindow
    {
    public:
        /** Constructor. OnClose is invoked by the close button; the title bar drags Window natively (hit-testing). */
        HelloWindow(std::function<void()> OnClose, luvk::platform::Window& Window);

        /** Destructor. Removes the hit-test callback that points to this object. */
        ~HelloWindow();

        /** Non-copyable: the window keeps a callback bound to this object. */
        HelloWindow(const HelloWindow&) = delete;

        /** Non-copyable. */
        HelloWindow& operator=(const HelloWindow&) = delete;

        /** Advances the glitch/scanline animation. */
        void Update(float DeltaTime);

        /** Draws the full native window. */
        void Draw();

    private:
        /** Draws the title bar (drag area, status and close button). */
        void DrawTitleBar();

        /** Draws the animated HELLO WORLD title. */
        void DrawTitle() const;

        /** Draws the status lines, progress bar, PING button and footer. */
        void DrawBody();

        /** True inside the title bar but outside the close button (where the OS should drag the window). */
        [[nodiscard]] bool IsDragArea(float X, float Y) const noexcept;

        /** Window-relative point (X, Y) in design units, scaled by the display scale. */
        [[nodiscard]] ImVec2 At(float X, float Y) const;

        /** Design units -> pixels at the current display scale. */
        [[nodiscard]] float Scaled(float Value) const;

        /** Callback invoked by the close button. */
        std::function<void()> m_OnClose;

        /** Native window dragged by the title bar. */
        luvk::platform::Window* m_Window = nullptr;

        /** Title bar drag area of the last frame, in window coordinates. */
        ImVec2 m_DragMin = {};

        /** Bottom-right corner of the drag area. */
        ImVec2 m_DragMax = {};

        /** Close button of the last frame, in window coordinates (excluded from the drag area). */
        ImVec2 m_CloseMin = {};

        /** Bottom-right corner of the close button. */
        ImVec2 m_CloseMax = {};

        /** Animation time in seconds. */
        float m_Time = 0.F;

        /** Remaining seconds of glitch effect forced by the PING button. */
        float m_PingTime = 0.F;
    };
}
