// Author: Lucas Vilas-Boas
// Year: 2026
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "TemplateApp.hpp"
#include "CyberStyle.hpp"
#include "HelloWindow.hpp"

// A UI-only application: no scene, camera or statistics. luvk persists the layout and the pipeline cache in the
// per-user data directory derived from Organization/AppName.
Template::TemplateApp::TemplateApp()
    : Application({.Title           = "LU//VK NEON TERMINAL",
                   .Width           = 460U,
                   .Height          = 240U,
                   .Borderless      = true,
                   .GpuTimings      = false,
                   .ShowStatistics  = false,
                   .RenderScene     = false,
                   .EnableDocking   = false,
                   .EnableViewports = false,
                   .BackgroundColor = {0.004F, 0.008F, 0.016F, 1.F},
                   .Organization    = "lucoiso",
                   .AppName         = "luvk-imgui-template"})
{
}

Template::TemplateApp::~TemplateApp() = default;

void Template::TemplateApp::OnInit()
{
    CyberStyle::Apply();
    m_HelloWindow = std::make_unique<HelloWindow>(
        [this]
        {
            RequestExit();
        },
        GetWindow());
}

void Template::TemplateApp::OnUpdate(const float DeltaTime)
{
    m_HelloWindow->Update(DeltaTime);
}

void Template::TemplateApp::OnGui()
{
    m_HelloWindow->Draw();
}
