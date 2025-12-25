// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/WindowUI.hpp"
#include <imgui.h>
#include <mutex>
#include "UserInterface/Libraries/ButtonUI.hpp"

using namespace UserInterface;

void Window::Overlay(std::function<void()>&& Header, std::function<void()>&& Content, std::function<void()>&& Footer)
{
    static ImGuiWindowClass WindowClass{};
    static std::once_flag   CallFlag{};
    std::call_once(CallFlag,
                   []
                   {
                       WindowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoTaskBarIcon |
                               ImGuiViewportFlags_TopMost |
                               ImGuiViewportFlags_NoDecoration;
                   });

    ImGui::SetNextWindowClass(&WindowClass);
    ImGui::SetNextWindowSize(ImVec2(300.0F, 100.0F), ImGuiCond_FirstUseEver);

    constexpr ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoTitleBar;

    if (ImGui::Begin("Overlay", nullptr, WindowFlags))
    {
        ImDrawList* const DrawList = ImGui::GetWindowDrawList();
        const ImVec2      Pos      = ImGui::GetWindowPos();
        const ImVec2      Size     = ImGui::GetWindowSize();
        const ImVec2      PMax(Pos.x + Size.x, Pos.y + Size.y);

        DrawList->AddRectFilled(Pos,
                                ImVec2(Pos.x + 3.0F, PMax.y),
                                ImColor(0.85F, 0.15F, 0.18F, 1.0F));

        ImGui::SetCursorPos(ImVec2(Size.x - 30.0F, 8.0F));
        Button::CloseApp();

        ImGui::SetCursorPos(ImVec2(15.0F, 15.0F));
        ImGui::BeginGroup();
        {
            Header();

            const float LineY = ImGui::GetCursorScreenPos().y;
            DrawList->AddLine(ImVec2(Pos.x + 10.0F, LineY),
                              ImVec2(PMax.x - 10.0F, LineY),
                              ImColor(0.85F, 0.15F, 0.18F, 0.4F),
                              1.0F);

            ImGui::Dummy(ImVec2(0.0F, 8.0F));

            Content();

            ImGui::Dummy(ImVec2(0.0F, 4.0F));
            ImGui::SetCursorPosY(Size.y - 35.0F);

            Footer();
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}