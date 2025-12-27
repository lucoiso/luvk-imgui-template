// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/WindowUI.hpp"
#include <mutex>
#include "UserInterface/Libraries/ControlUI.hpp"
#include "UserInterface/Libraries/ThemeUI.hpp"

using namespace UserInterface;

Window::Overlay::Overlay(const char* Name, bool* State)
    : m_State(State),
      m_Name(Name) {}

Window::Overlay::~Overlay()
{
    static ImGuiWindowClass WindowClass;
    static std::once_flag   Init;

    std::call_once(Init,
                   []
                   {
                       WindowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoDecoration | ImGuiViewportFlags_TopMost;
                   });

    ImGui::SetNextWindowClass(&WindowClass);

    if (ImGui::Begin(m_Name, m_State, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImDrawList*  DL   = ImGui::GetWindowDrawList();
        const ImVec2 Pos  = ImGui::GetWindowPos();
        const ImVec2 Size = ImGui::GetWindowSize();

        DL->AddRectFilled(Pos, ImVec2(Pos.x + 4, Pos.y + Size.y), Theme::Colors::BrightRed);

        ImGui::SetCursorPos(ImVec2(Size.x - 30, 10));
        if (Control::Button("X", ImVec2(22, 22)))
        {
            if (m_State)
            {
                *m_State = false;
            }
        }

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::BeginGroup();
        for (const auto& RenderCall : m_Content)
        {
            RenderCall();
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}

void Window::Overlay::AddContent(std::function<void()>&& Function)
{
    m_Content.push_back(std::move(Function));
}
