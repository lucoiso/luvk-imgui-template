// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/ThemeUI.hpp"

using namespace UserInterface;

void Theme::PushDefaultTheme()
{
    ImGuiStyle&   Style  = ImGui::GetStyle();
    ImVec4* const Colors = Style.Colors;

    Style.WindowRounding              = 0.0F;
    Style.ChildRounding               = 0.0F;
    Style.FrameRounding               = 0.0F;
    Style.PopupRounding               = 0.0F;
    Style.TabRounding                 = 0.0F;
    Style.ScrollbarRounding           = 0.0F;
    Style.GrabRounding                = 0.0F;
    Style.WindowBorderSize            = 1.0F;
    Style.FrameBorderSize             = 0.0F;
    Style.PopupBorderSize             = 1.0F;
    Style.WindowPadding               = ImVec2(12.0F, 12.0F);
    Style.ItemSpacing                 = ImVec2(8.0F, 4.0F);
    Style.ItemInnerSpacing            = ImVec2(4.0F, 4.0F);
    Style.DisplaySafeAreaPadding      = ImVec2(0.0F, 0.0F);
    Colors[ImGuiCol_WindowBg]         = Colors::Black;
    Colors[ImGuiCol_Border]           = Colors::DeepRed;
    Colors[ImGuiCol_TitleBg]          = Colors::Black;
    Colors[ImGuiCol_TitleBgActive]    = Colors::Black;
    Colors[ImGuiCol_TitleBgCollapsed] = Colors::Black;
    Colors[ImGuiCol_Text]             = Colors::TextWhite;
    Colors[ImGuiCol_Button]           = Colors::DarkBlue;
    Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.15F, 0.25F, 0.40F, 1.00F);
    Colors[ImGuiCol_ButtonActive]     = Colors::Cyan;
    Colors[ImGuiCol_Separator]        = Colors::DeepRed;
    Colors[ImGuiCol_SeparatorHovered] = Colors::BrightRed;
    Colors[ImGuiCol_SeparatorActive]  = Colors::BrightRed;
    Colors[ImGuiCol_FrameBg]          = ImVec4(0.07F, 0.07F, 0.07F, 1.00F);
    Colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.10F, 0.10F, 0.10F, 1.00F);
    Colors[ImGuiCol_FrameBgActive]    = ImVec4(0.15F, 0.15F, 0.15F, 1.00F);
    Colors[ImGuiCol_CheckMark]        = Colors::Cyan;
    Colors[ImGuiCol_SliderGrab]       = Colors::BrightRed;
    Colors[ImGuiCol_SliderGrabActive] = Colors::Cyan;
}
