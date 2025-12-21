// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/UserInterface/ImGuiLayer.hpp"
#include <imgui.h>

using namespace Core;

void ImGuiLayer::Draw()
{
    ImGuiLayerBase::Draw();

    static bool IsWindowOpen = true;

    if (IsWindowOpen)
    {
        ImGuiWindowClass WindowClass{};
        WindowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_TopMost;
        ImGui::SetNextWindowClass(&WindowClass);

        if (constexpr ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
            ImGui::Begin("Overlay", &IsWindowOpen, WindowFlags))
        {
            ImDrawList* const DrawList = ImGui::GetWindowDrawList();

            ImGui::SetCursorPos(ImVec2(16.0F, 44.0F));
            const ImVec2 TextPos = ImGui::GetCursorScreenPos();

            const ImVec2 WindowPos  = ImGui::GetWindowPos();
            const ImVec2 WindowSize = ImGui::GetWindowSize();

            DrawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 28.0F),
                              ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + 28.0F),
                              ImGui::ColorConvertFloat4ToU32(ImVec4(0.85F, 0.15F, 0.18F, 1.0F)),
                              1.0F);

            constexpr ImVec4 GlowSoft(0.25F, 0.85F, 0.85F, 0.18F);
            constexpr auto   Text = "Hello World";

            DrawList->AddText(ImGui::GetFont(),
                              ImGui::GetFontSize(),
                              ImVec2(TextPos.x - 1.0F, TextPos.y),
                              ImGui::ColorConvertFloat4ToU32(GlowSoft),
                              Text);

            DrawList->AddText(ImGui::GetFont(),
                              ImGui::GetFontSize(),
                              ImVec2(TextPos.x + 1.0F, TextPos.y),
                              ImGui::ColorConvertFloat4ToU32(GlowSoft),
                              Text);

            DrawList->AddText(ImGui::GetFont(),
                              ImGui::GetFontSize(),
                              ImVec2(TextPos.x, TextPos.y - 1.0F),
                              ImGui::ColorConvertFloat4ToU32(GlowSoft),
                              Text);

            DrawList->AddText(ImGui::GetFont(),
                              ImGui::GetFontSize(),
                              ImVec2(TextPos.x, TextPos.y + 1.0F),
                              ImGui::ColorConvertFloat4ToU32(GlowSoft),
                              Text);

            DrawList->AddLine(ImVec2(WindowPos.x + WindowSize.x - 18.0F, WindowPos.y + WindowSize.y),
                              ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y - 18.0F),
                              ImGui::ColorConvertFloat4ToU32(ImVec4(0.85F, 0.15F, 0.18F, 1.0F)),
                              2.0F);

            ImGui::TextColored(ImVec4(0.0F, 0.94F, 1.0F, 1.0F), Text);
        }
        ImGui::End();
    }
    else
    {
        SDL_Event QuitEvent;
        QuitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&QuitEvent);
    }
}

void ImGuiLayer::PushStyle() const
{
    ImGuiStyle& ImGuiStyle = ImGui::GetStyle();

    ImGuiStyle.Alpha         = 1.0F;
    ImGuiStyle.DisabledAlpha = 0.35F;

    ImGuiStyle.WindowPadding            = ImVec2(14.0F, 12.0F);
    ImGuiStyle.WindowRounding           = 0.0F;
    ImGuiStyle.WindowBorderSize         = 1.0F;
    ImGuiStyle.WindowMinSize            = ImVec2(40.0F, 40.0F);
    ImGuiStyle.WindowTitleAlign         = ImVec2(0.0F, 0.5F);
    ImGuiStyle.WindowMenuButtonPosition = ImGuiDir_Right;

    ImGuiStyle.ChildRounding   = 0.0F;
    ImGuiStyle.ChildBorderSize = 1.0F;

    ImGuiStyle.PopupRounding   = 0.0F;
    ImGuiStyle.PopupBorderSize = 1.0F;

    ImGuiStyle.FramePadding    = ImVec2(14.0F, 6.0F);
    ImGuiStyle.FrameRounding   = 0.0F;
    ImGuiStyle.FrameBorderSize = 1.0F;

    ImGuiStyle.ItemSpacing      = ImVec2(10.0F, 6.0F);
    ImGuiStyle.ItemInnerSpacing = ImVec2(8.0F, 4.0F);

    ImGuiStyle.ScrollbarSize     = 8.0F;
    ImGuiStyle.ScrollbarRounding = 0.0F;

    ImGuiStyle.GrabMinSize  = 6.0F;
    ImGuiStyle.GrabRounding = 0.0F;

    ImGuiStyle.TabRounding   = 0.0F;
    ImGuiStyle.TabBorderSize = 1.0F;

    ImGuiStyle.ColorButtonPosition = ImGuiDir_Right;
    ImGuiStyle.ButtonTextAlign     = ImVec2(0.5F, 0.5F);
    ImGuiStyle.SelectableTextAlign = ImVec2(0.0F, 0.5F);

    ImVec4* const ImGuiColors = ImGuiStyle.Colors;

    constexpr ImVec4 DeepBlack(0.03F, 0.03F, 0.035F, 1.0F);
    constexpr ImVec4 PanelBlack(0.06F, 0.06F, 0.07F, 1.0F);
    constexpr ImVec4 RedPrimary(0.85F, 0.15F, 0.18F, 1.0F);
    constexpr ImVec4 RedSoft(0.55F, 0.12F, 0.14F, 1.0F);
    constexpr ImVec4 CyanAccent(0.25F, 0.85F, 0.85F, 1.0F);
    constexpr ImVec4 CyanDim(0.20F, 0.55F, 0.60F, 1.0F);
    constexpr ImVec4 BorderRed(0.75F, 0.18F, 0.20F, 0.75F);

    ImGuiColors[ImGuiCol_Text]         = CyanAccent;
    ImGuiColors[ImGuiCol_TextDisabled] = ImVec4(0.40F, 0.40F, 0.42F, 1.0F);

    ImGuiColors[ImGuiCol_WindowBg] = DeepBlack;
    ImGuiColors[ImGuiCol_ChildBg]  = PanelBlack;
    ImGuiColors[ImGuiCol_PopupBg]  = PanelBlack;

    ImGuiColors[ImGuiCol_Border]       = BorderRed;
    ImGuiColors[ImGuiCol_BorderShadow] = ImVec4(0.0F, 0.0F, 0.0F, 0.0F);

    ImGuiColors[ImGuiCol_FrameBg]        = ImVec4(0.10F, 0.05F, 0.06F, 1.0F);
    ImGuiColors[ImGuiCol_FrameBgHovered] = ImVec4(0.18F, 0.07F, 0.08F, 1.0F);
    ImGuiColors[ImGuiCol_FrameBgActive]  = RedPrimary;

    ImGuiColors[ImGuiCol_TitleBg]          = DeepBlack;
    ImGuiColors[ImGuiCol_TitleBgActive]    = DeepBlack;
    ImGuiColors[ImGuiCol_TitleBgCollapsed] = DeepBlack;

    ImGuiColors[ImGuiCol_MenuBarBg] = DeepBlack;

    ImGuiColors[ImGuiCol_ScrollbarBg]          = DeepBlack;
    ImGuiColors[ImGuiCol_ScrollbarGrab]        = RedSoft;
    ImGuiColors[ImGuiCol_ScrollbarGrabHovered] = RedPrimary;
    ImGuiColors[ImGuiCol_ScrollbarGrabActive]  = RedPrimary;

    ImGuiColors[ImGuiCol_CheckMark]        = CyanAccent;
    ImGuiColors[ImGuiCol_SliderGrab]       = CyanDim;
    ImGuiColors[ImGuiCol_SliderGrabActive] = CyanAccent;

    ImGuiColors[ImGuiCol_Button]        = ImVec4(0.10F, 0.05F, 0.06F, 1.0F);
    ImGuiColors[ImGuiCol_ButtonHovered] = ImVec4(0.20F, 0.08F, 0.10F, 1.0F);
    ImGuiColors[ImGuiCol_ButtonActive]  = RedPrimary;

    ImGuiColors[ImGuiCol_Header]        = ImVec4(0.12F, 0.06F, 0.07F, 1.0F);
    ImGuiColors[ImGuiCol_HeaderHovered] = ImVec4(0.25F, 0.10F, 0.12F, 1.0F);
    ImGuiColors[ImGuiCol_HeaderActive]  = RedPrimary;

    ImGuiColors[ImGuiCol_Separator]        = BorderRed;
    ImGuiColors[ImGuiCol_SeparatorHovered] = RedPrimary;
    ImGuiColors[ImGuiCol_SeparatorActive]  = RedPrimary;

    ImGuiColors[ImGuiCol_Tab]                = DeepBlack;
    ImGuiColors[ImGuiCol_TabHovered]         = ImVec4(0.18F, 0.08F, 0.10F, 1.0F);
    ImGuiColors[ImGuiCol_TabActive]          = ImVec4(0.22F, 0.10F, 0.12F, 1.0F);
    ImGuiColors[ImGuiCol_TabUnfocused]       = DeepBlack;
    ImGuiColors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12F, 0.06F, 0.07F, 1.0F);

    ImGuiColors[ImGuiCol_TextSelectedBg] = ImVec4(0.85F, 0.15F, 0.18F, 0.35F);

    ImGuiColors[ImGuiCol_NavHighlight]     = CyanAccent;
    ImGuiColors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0F, 0.0F, 0.0F, 0.85F);
}
