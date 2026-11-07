// Author: Lucas Vilas-Boas
// Year: 2026
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "CyberStyle.hpp"
#include <imgui.h>

void Template::CyberStyle::Apply()
{
    ImGuiStyle& Style = ImGui::GetStyle();
    ImGui::StyleColorsDark(&Style);

    Style.WindowRounding    = 2.F;
    Style.ChildRounding     = 2.F;
    Style.FrameRounding     = 2.F;
    Style.PopupRounding     = 2.F;
    Style.TabRounding       = 2.F;
    Style.ScrollbarRounding = 2.F;
    Style.GrabRounding      = 2.F;
    Style.WindowBorderSize  = 1.F;
    Style.FrameBorderSize   = 1.F;
    Style.WindowPadding     = ImVec2(14.F, 12.F);
    Style.FramePadding      = ImVec2(8.F, 4.F);
    Style.ItemSpacing       = ImVec2(8.F, 6.F);
    Style.WindowTitleAlign  = ImVec2(0.5F, 0.5F);

    ImVec4* const Colors              = Style.Colors;
    Colors[ImGuiCol_Text]             = ImVec4(0.82F, 0.95F, 1.00F, 1.00F);
    Colors[ImGuiCol_TextDisabled]     = ImVec4(0.30F, 0.45F, 0.55F, 1.00F);
    Colors[ImGuiCol_WindowBg]         = ImVec4(0.015F, 0.025F, 0.045F, 0.97F);
    Colors[ImGuiCol_ChildBg]          = ImVec4(0.02F, 0.03F, 0.05F, 1.00F);
    Colors[ImGuiCol_PopupBg]          = ImVec4(0.015F, 0.025F, 0.045F, 0.98F);
    Colors[ImGuiCol_Border]           = ImVec4(0.00F, 0.85F, 1.00F, 0.70F);
    Colors[ImGuiCol_BorderShadow]     = ImVec4(1.00F, 0.10F, 0.60F, 0.20F);
    Colors[ImGuiCol_FrameBg]          = ImVec4(0.03F, 0.06F, 0.10F, 1.00F);
    Colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.05F, 0.12F, 0.18F, 1.00F);
    Colors[ImGuiCol_FrameBgActive]    = ImVec4(0.00F, 0.65F, 0.85F, 0.60F);
    Colors[ImGuiCol_TitleBg]          = ImVec4(0.01F, 0.02F, 0.04F, 1.00F);
    Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.03F, 0.06F, 0.10F, 1.00F);
    Colors[ImGuiCol_MenuBarBg]        = ImVec4(0.01F, 0.02F, 0.04F, 1.00F);
    Colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.01F, 0.02F, 0.04F, 1.00F);
    Colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.00F, 0.55F, 0.75F, 1.00F);
    Colors[ImGuiCol_CheckMark]        = ImVec4(1.00F, 0.10F, 0.60F, 1.00F);
    Colors[ImGuiCol_SliderGrab]       = ImVec4(0.00F, 0.85F, 1.00F, 1.00F);
    Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00F, 0.10F, 0.60F, 1.00F);
    Colors[ImGuiCol_Button]           = ImVec4(0.03F, 0.10F, 0.16F, 1.00F);
    Colors[ImGuiCol_ButtonHovered]    = ImVec4(1.00F, 0.10F, 0.60F, 0.75F);
    Colors[ImGuiCol_ButtonActive]     = ImVec4(0.00F, 0.85F, 1.00F, 0.90F);
    Colors[ImGuiCol_Header]           = ImVec4(0.03F, 0.12F, 0.18F, 1.00F);
    Colors[ImGuiCol_HeaderHovered]    = ImVec4(0.00F, 0.55F, 0.75F, 0.70F);
    Colors[ImGuiCol_HeaderActive]     = ImVec4(0.00F, 0.85F, 1.00F, 0.90F);
    Colors[ImGuiCol_Separator]        = ImVec4(0.00F, 0.55F, 0.75F, 0.60F);
    Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00F, 0.10F, 0.60F, 1.00F);
    Colors[ImGuiCol_SeparatorActive]  = ImVec4(0.00F, 0.85F, 1.00F, 1.00F);
    Colors[ImGuiCol_Tab]              = ImVec4(0.02F, 0.05F, 0.08F, 1.00F);
    Colors[ImGuiCol_TabHovered]       = ImVec4(1.00F, 0.10F, 0.60F, 0.60F);
    Colors[ImGuiCol_TabSelected]      = ImVec4(0.00F, 0.65F, 0.85F, 0.60F);
    Colors[ImGuiCol_DockingPreview]   = ImVec4(0.00F, 0.85F, 1.00F, 0.50F);
    Colors[ImGuiCol_DockingEmptyBg]   = ImVec4(0.01F, 0.02F, 0.04F, 1.00F);
}
