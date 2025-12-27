/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#include "UserInterface/Layers/AppUI.hpp"
#include <SDL3/SDL.h>
#include "UserInterface/Libraries/AnimationUI.hpp"
#include "UserInterface/Libraries/ControlUI.hpp"
#include "UserInterface/Libraries/DataUI.hpp"
#include "UserInterface/Libraries/EffectUI.hpp"
#include "UserInterface/Libraries/LayoutUI.hpp"
#include "UserInterface/Libraries/ThemeUI.hpp"
#include "UserInterface/Libraries/WindowUI.hpp"

using namespace UserInterface;

bool AppUI::Draw()
{
    static bool Active = true;
    if (!Active)
    {
        return false;
    }

    SDL_Rect DisplayBounds;
    if (SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &DisplayBounds))
    {
        constexpr ImVec2 WindowSize(600.0F, 400.0F);
        constexpr float  Offset = 15.0F;

        const ImVec2 TargetPos{static_cast<float>(DisplayBounds.x + DisplayBounds.w) - WindowSize.x - Offset,
                               static_cast<float>(DisplayBounds.y) + Offset};

        ImGui::SetNextWindowSize(WindowSize, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(TargetPos, ImGuiCond_FirstUseEver);
    }

    Window::Overlay HUD("Diagnostic_System", &Active);

    HUD.AddContent([]
    {
        Effect::Unfocus UnfocusEffect;
        Data::Text("SYSTEM DIAGNOSTICS", Data::TextType::Title, Theme::Colors::Cyan, true);
        Layout::Separator();
    });

    HUD.AddContent([]
    {
        const Animation::FlickerAnimation Flicker(8.0F);
        ImColor                           StatusColor = Theme::Colors::BrightRed;
        StatusColor.Value.w                           = Flicker.GetValue();

        Data::KeyValue("NODE_LOGIC", "ACTIVE", StatusColor);
        Layout::VerticalSpacing(5.0F);
    });

    HUD.AddContent([]
    {
        const Layout::IndentScope Indent(10.0F);

        Data::KeyValue("CORE_TEMP", "42°C", Theme::Colors::Yellow);
        Data::KeyValue("MEM_LOAD", "2.4 GB", Theme::Colors::Yellow);

        Layout::VerticalSpacing(10.0F);
    });

    HUD.AddContent([]
    {
        static float                    Progress = 0.65F;
        const Animation::PulseAnimation Pulse(3.0F);

        ImColor BarColor = Theme::Colors::Cyan;
        BarColor.Value.w = 0.4F + 0.6F * Pulse.GetValue();

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, static_cast<ImVec4>(BarColor));
        Control::ProgressBar(Progress, "SYNCING_DATA...");
        ImGui::PopStyleColor();

        Layout::VerticalSpacing(10.0F);
    });

    HUD.AddContent([]
    {
        {
            ImColor EnergyColor = Theme::Colors::Cyan;
            EnergyColor.Value.w = 0.3F;

            Effect::Scanline Energy(EnergyColor, 6.0F, 20.0F, 2.5F);

            static bool Optics = true;
            Control::Toggle("ENHANCED_OPTICS", &Optics);
        }

        {
            Effect::Scanline Classic(ImColor(0, 0, 0, 150), 3.0F, 5.0F, 1.0F);

            static bool Stabilizer = false;
            Control::Toggle("STABILIZER_GYRO", &Stabilizer);
        }

        Layout::VerticalSpacing(5.0F);

        if (Control::Button("RESET_CORE_MODULE", ImVec2(-1, 30))) {}
    });

    HUD.AddContent([]
    {
        {
            Effect::Glow WarningGlow(Theme::Colors::BrightRed, 16.0F);
            if (Control::Button("CRITICAL_OVERRIDE", ImVec2(-1, 40))) {}
        }

        {
            [[maybe_unused]] Effect::Glitch HeavyGlitch(3.0F);
            Data::Text("DATA_STREAM_01: STABLE", Data::TextType::Footer, Theme::Colors::Cyan);
        }
    });

    HUD.AddContent([]
    {
        const float WindowHeight = ImGui::GetWindowSize().y;
        ImGui::SetCursorPosY(WindowHeight - 25.0F);

        Data::Text("V_OS Ver. 2.0.7.7 [STABLE]", Data::TextType::Footer, Theme::Colors::Yellow);
    });

    return Active;
}
