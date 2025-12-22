// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Layers/Overlay.hpp"
#include <imgui.h>
#include <SDL3/SDL_events.h>

using namespace UserInterface;

void Overlay::Draw()
{
    Core::ImGuiLayerBase::Draw();

    static bool IsWindowOpen = true;

    if (IsWindowOpen)
    {
        ImGuiWindowClass WindowClass{};
        WindowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoTaskBarIcon |
                ImGuiViewportFlags_TopMost |
                ImGuiViewportFlags_NoDecoration;

        ImGui::SetNextWindowClass(&WindowClass);
        ImGui::SetNextWindowSize(ImVec2(300.0F, 100.0F), ImGuiCond_FirstUseEver);

        constexpr ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoTitleBar;

        if (ImGui::Begin("Overlay", &IsWindowOpen, WindowFlags))
        {
            ImDrawList* const DrawList = ImGui::GetWindowDrawList();
            const ImVec2      Pos      = ImGui::GetWindowPos();
            const ImVec2      Size     = ImGui::GetWindowSize();
            const ImVec2      PMax(Pos.x + Size.x, Pos.y + Size.y);

            DrawList->AddRectFilled(Pos,
                                    ImVec2(Pos.x + 3.0F, PMax.y),
                                    ImColor(0.85F, 0.15F, 0.18F, 1.0F));

            ImGui::SetCursorPos(ImVec2(Size.x - 30.0F, 8.0F));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.08F, 0.15F, 0.25F, 1.0F));
            if (ImGui::Button("X", ImVec2(22.0F, 22.0F)))
            {
                IsWindowOpen = false;
            }
            ImGui::PopStyleColor();

            ImGui::SetCursorPos(ImVec2(15.0F, 15.0F));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35F, 0.90F, 0.90F, 1.0F));
                ImGui::Text("HELLO");
                ImGui::PopStyleColor();

                const float LineY = ImGui::GetCursorScreenPos().y;
                DrawList->AddLine(ImVec2(Pos.x + 10.0F, LineY),
                                  ImVec2(PMax.x - 10.0F, LineY),
                                  ImColor(0.85F, 0.15F, 0.18F, 0.4F),
                                  1.0F);

                ImGui::Dummy(ImVec2(0.0F, 8.0F));
                ImGui::TextColored(ImVec4(0.85F, 0.20F, 0.22F, 1.0F), "WORLD");
                ImGui::Dummy(ImVec2(0.0F, 4.0F));

                ImGui::SetCursorPosY(Size.y - 35.0F);
                ImGui::TextColored(ImVec4(0.95F, 0.85F, 0.10F, 1.0F), "E$");
                ImGui::SameLine(0.0F, 10.0F);
                ImGui::Text("2025");
            }
            ImGui::EndGroup();
        }
        ImGui::End();
    }
    else
    {
        SDL_Event QuitEvent{.type = SDL_EVENT_QUIT};
        SDL_PushEvent(&QuitEvent);
    }
}

void Overlay::PushStyle() const
{
    ImGuiStyle&   Style  = ImGui::GetStyle();
    ImVec4* const Colors = Style.Colors;

    Style.WindowRounding    = 0.0F;
    Style.ChildRounding     = 0.0F;
    Style.FrameRounding     = 0.0F;
    Style.PopupRounding     = 0.0F;
    Style.TabRounding       = 0.0F;
    Style.ScrollbarRounding = 0.0F;
    Style.GrabRounding      = 0.0F;

    Style.WindowBorderSize = 1.0F;
    Style.FrameBorderSize  = 0.0F;
    Style.PopupBorderSize  = 1.0F;

    Style.WindowPadding          = ImVec2(12.0F, 12.0F);
    Style.ItemSpacing            = ImVec2(8.0F, 4.0F);
    Style.ItemInnerSpacing       = ImVec2(4.0F, 4.0F);
    Style.DisplaySafeAreaPadding = ImVec2(0.0F, 0.0F);

    constexpr ImVec4 Black(0.04F, 0.04F, 0.04F, 1.00F);
    constexpr ImVec4 DeepRed(0.45F, 0.00F, 0.00F, 1.00F);
    constexpr ImVec4 BrightRed(0.85F, 0.12F, 0.12F, 1.00F);
    constexpr ImVec4 Cyan(0.45F, 0.95F, 0.95F, 1.00F);
    constexpr ImVec4 Yellow(0.95F, 0.85F, 0.15F, 1.00F);
    constexpr ImVec4 DarkBlue(0.08F, 0.15F, 0.25F, 1.00F);
    constexpr ImVec4 TextWhite(0.95F, 0.95F, 0.95F, 1.00F);

    Colors[ImGuiCol_WindowBg] = Black;
    Colors[ImGuiCol_Border]   = DeepRed;

    Colors[ImGuiCol_TitleBg]          = Black;
    Colors[ImGuiCol_TitleBgActive]    = Black;
    Colors[ImGuiCol_TitleBgCollapsed] = Black;
    Colors[ImGuiCol_Text]             = TextWhite;

    Colors[ImGuiCol_Button]        = DarkBlue;
    Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15F, 0.25F, 0.40F, 1.00F);
    Colors[ImGuiCol_ButtonActive]  = Cyan;

    Colors[ImGuiCol_Separator]        = DeepRed;
    Colors[ImGuiCol_SeparatorHovered] = BrightRed;
    Colors[ImGuiCol_SeparatorActive]  = BrightRed;

    Colors[ImGuiCol_FrameBg]        = ImVec4(0.07F, 0.07F, 0.07F, 1.00F);
    Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10F, 0.10F, 0.10F, 1.00F);
    Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.15F, 0.15F, 0.15F, 1.00F);

    Colors[ImGuiCol_CheckMark]        = Cyan;
    Colors[ImGuiCol_SliderGrab]       = BrightRed;
    Colors[ImGuiCol_SliderGrabActive] = Cyan;
}
