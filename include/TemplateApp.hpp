// Author: Lucas Vilas-Boas
// Year: 2026
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <memory>
#include <luvk/App/Application.hpp>

namespace Template
{
    class HelloWindow;

    /** Minimal luvk application: a single cyberpunk Dear ImGui window rendered entirely by luvk. */
    class TemplateApp final : public luvk::app::Application
    {
    public:
        /** Constructor: configures a small, UI-only borderless window. */
        TemplateApp();

        /** Destructor (defined in the .cpp where HelloWindow is complete). */
        ~TemplateApp() override;

    protected:
        /** Applies the neon theme and creates the hello window. */
        void OnInit() override;

        /** Advances the window animation. */
        void OnUpdate(float DeltaTime) override;

        /** Draws the hello window. */
        void OnGui() override;

    private:
        /** The cyberpunk hello window. */
        std::unique_ptr<HelloWindow> m_HelloWindow;
    };
}
