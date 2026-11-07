// Author: Lucas Vilas-Boas
// Year: 2026
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "HelloWindow.hpp"
#include <algorithm>
#include <cmath>
#include <utility>
#include <luvk/Platform/Window.hpp>

namespace
{
    constexpr ImU32 Background   = IM_COL32(4, 7, 14, 255);
    constexpr ImU32 BackgroundHi = IM_COL32(10, 18, 32, 255);
    constexpr ImU32 Cyan         = IM_COL32(0, 235, 255, 255);
    constexpr ImU32 CyanDim      = IM_COL32(0, 120, 150, 90);
    constexpr ImU32 Magenta      = IM_COL32(255, 30, 160, 255);
    constexpr ImU32 Text         = IM_COL32(205, 240, 255, 255);
    constexpr ImU32 TextDim      = IM_COL32(90, 140, 160, 255);

    /** Height of the title bar, in design units. */
    constexpr float TitleBarHeight = 30.F;

    /** Size of the close button, in design units. */
    constexpr float CloseSize = 17.F;
}

Template::HelloWindow::HelloWindow(std::function<void()> OnClose, luvk::platform::Window& Window)
    : m_OnClose(std::move(OnClose)),
      m_Window(&Window)
{
    // The OS moves the window while the title bar is dragged: native snapping, no per-frame repositioning.
    m_Window->SetHitTest(
        [this](const float X, const float Y)
        {
            return IsDragArea(X, Y) ? luvk::platform::HitTest::Draggable : luvk::platform::HitTest::Normal;
        });
}

Template::HelloWindow::~HelloWindow()
{
    m_Window->SetHitTest({});
}

void Template::HelloWindow::Update(const float DeltaTime)
{
    m_Time     = std::fmod(m_Time + DeltaTime, 3600.F);
    m_PingTime = std::max(0.F, m_PingTime - DeltaTime);
}

float Template::HelloWindow::Scaled(const float Value) const
{
    // Fonts already follow the display scale; the hand-placed geometry scales with them.
    return Value * ImGui::GetStyle().FontScaleDpi;
}

ImVec2 Template::HelloWindow::At(const float X, const float Y) const
{
    const ImVec2 Origin = ImGui::GetWindowPos();
    return {Origin.x + Scaled(X), Origin.y + Scaled(Y)};
}

bool Template::HelloWindow::IsDragArea(const float X, const float Y) const noexcept
{
    const auto Inside = [X, Y](const ImVec2 Min, const ImVec2 Max)
    {
        return X >= Min.x && X < Max.x && Y >= Min.y && Y < Max.y;
    };
    return Inside(m_DragMin, m_DragMax) && !Inside(m_CloseMin, m_CloseMax);
}

void Template::HelloWindow::Draw()
{
    // The native window is exactly the size of this ImGui window, so no native background is visible.
    const ImGuiViewport* const Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(Viewport->Pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(Viewport->Size, ImGuiCond_Always);

    constexpr ImGuiWindowFlags Flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.F, 0.F));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.F);

    if (ImGui::Begin("##hello_window", nullptr, Flags))
    {
        const ImVec2      Position = ImGui::GetWindowPos();
        const ImVec2      Size     = ImGui::GetWindowSize();
        const ImVec2      Max(Position.x + Size.x, Position.y + Size.y);
        ImDrawList* const Draw   = ImGui::GetWindowDrawList();
        const float       Inset  = Scaled(2.F);
        const float       Margin = Scaled(3.F);

        Draw->AddRectFilledMultiColor(Position, Max, BackgroundHi, Background, Background, BackgroundHi);
        Draw->AddRect(Position, Max, Cyan, Scaled(2.F), Scaled(1.2F));
        Draw->AddRect(ImVec2(Position.x + Inset, Position.y + Inset), ImVec2(Max.x - Inset, Max.y - Inset), CyanDim, Scaled(2.F), Scaled(1.F));

        // Corner brackets.
        const float Bracket   = Scaled(18.F);
        const float Thickness = Scaled(2.F);
        Draw->AddLine(Position, ImVec2(Position.x + Bracket, Position.y), Magenta, Thickness);
        Draw->AddLine(Position, ImVec2(Position.x, Position.y + Bracket), Magenta, Thickness);
        Draw->AddLine(ImVec2(Max.x, Position.y), ImVec2(Max.x - Bracket, Position.y), Magenta, Thickness);
        Draw->AddLine(ImVec2(Max.x, Position.y), ImVec2(Max.x, Position.y + Bracket), Magenta, Thickness);
        Draw->AddLine(ImVec2(Position.x, Max.y), ImVec2(Position.x + Bracket, Max.y), Magenta, Thickness);
        Draw->AddLine(ImVec2(Position.x, Max.y), ImVec2(Position.x, Max.y - Bracket), Magenta, Thickness);
        Draw->AddLine(ImVec2(Max.x, Max.y), ImVec2(Max.x - Bracket, Max.y), Magenta, Thickness);
        Draw->AddLine(ImVec2(Max.x, Max.y), ImVec2(Max.x, Max.y - Bracket), Magenta, Thickness);

        // Animated scanlines.
        const float Spacing = Scaled(4.F);
        const float Offset  = std::fmod(m_Time * Scaled(18.F), Spacing);
        for (float Y = Position.y + Offset; Y < Max.y; Y += Spacing)
        {
            Draw->AddLine(ImVec2(Position.x + Margin, Y), ImVec2(Max.x - Margin, Y), IM_COL32(0, 0, 0, 40));
        }

        DrawTitleBar();
        DrawTitle();
        DrawBody();

        // Bottom status strip.
        const ImVec2 StripMin(Position.x + Margin, Max.y - Scaled(18.F));
        Draw->AddRectFilled(StripMin, ImVec2(Max.x - Margin, Max.y - Margin), IM_COL32(0, 40, 55, 180));
        Draw->AddText(ImVec2(StripMin.x + Scaled(6.F), StripMin.y + Scaled(2.F)), Cyan, "VULKAN 1.3");
        Draw->AddText(ImVec2(StripMin.x + Scaled(96.F), StripMin.y + Scaled(2.F)), TextDim, "BINDLESS");

        constexpr const char* Chrome = "BORDERLESS + CUSTOM CHROME";
        Draw->AddText(ImVec2(Max.x - ImGui::CalcTextSize(Chrome).x - Scaled(12.F), StripMin.y + Scaled(2.F)), TextDim, Chrome);
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void Template::HelloWindow::DrawTitleBar()
{
    const ImVec2      Position = ImGui::GetWindowPos();
    const float       Width    = ImGui::GetWindowSize().x;
    ImDrawList* const Draw     = ImGui::GetWindowDrawList();
    const float       Margin   = Scaled(3.F);
    const float       Bottom   = Position.y + Scaled(TitleBarHeight);

    Draw->AddRectFilled(ImVec2(Position.x + Margin, Position.y + Margin), ImVec2(Position.x + Width - Margin, Bottom), IM_COL32(0, 28, 38, 220));
    Draw->AddLine(ImVec2(Position.x + Margin, Bottom), ImVec2(Position.x + Width - Margin, Bottom), CyanDim, 1.F);

    Draw->AddText(At(12.F, 9.F), Cyan, "LU//VK");
    Draw->AddText(At(72.F, 9.F), TextDim, "NEON TERMINAL  v1.0");

    // Remembered for the OS hit test, which runs between frames.
    m_DragMin = ImVec2(Position.x + Margin, Position.y + Margin);
    m_DragMax = ImVec2(Position.x + Width - Margin, Bottom);

    const float  Close = Scaled(CloseSize);
    const ImVec2 CloseMin(Position.x + Width - Scaled(27.F), Position.y + Scaled(7.F));
    const ImVec2 CloseMax(CloseMin.x + Close, CloseMin.y + Close);
    m_CloseMin = CloseMin;
    m_CloseMax = CloseMax;

    ImGui::SetCursorScreenPos(CloseMin);
    ImGui::InvisibleButton("##close", ImVec2(Close, Close));
    const bool  Hovered = ImGui::IsItemHovered();
    const bool  Pressed = ImGui::IsItemClicked();
    const float Cross   = Scaled(4.F);
    Draw->AddRect(CloseMin, CloseMax, Hovered ? Magenta : CyanDim, 1.F, 1.F);
    Draw->AddLine(ImVec2(CloseMin.x + Cross, CloseMin.y + Cross), ImVec2(CloseMax.x - Cross, CloseMax.y - Cross), Hovered ? Magenta : TextDim, Scaled(1.5F));
    Draw->AddLine(ImVec2(CloseMax.x - Cross, CloseMin.y + Cross), ImVec2(CloseMin.x + Cross, CloseMax.y - Cross), Hovered ? Magenta : TextDim, Scaled(1.5F));

    if (Pressed && m_OnClose)
    {
        m_OnClose();
    }
}

void Template::HelloWindow::DrawTitle() const
{
    const ImVec2      Position = ImGui::GetWindowPos();
    ImDrawList* const Draw     = ImGui::GetWindowDrawList();

    ImGui::PushFont(nullptr, 32.F);
    const ImVec2 TextSize = ImGui::CalcTextSize("HELLO WORLD");
    const ImVec2 TextPos(Position.x + (ImGui::GetWindowSize().x - TextSize.x) * 0.5F, Position.y + Scaled(44.F));

    const bool  Glitch = m_PingTime > 0.F || std::fmod(m_Time, 5.F) < 0.18F;
    const float Shift  = Glitch ? std::sin(m_Time * 173.F) * Scaled(3.F) : 0.F;
    const float Ghost  = Scaled(2.F);

    Draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(TextPos.x + Ghost, TextPos.y + Ghost), IM_COL32(0, 0, 0, 180), "HELLO WORLD");
    Draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(TextPos.x - Ghost + Shift, TextPos.y), IM_COL32(255, 30, 160, Glitch ? 220 : 90), "HELLO WORLD");
    Draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(TextPos.x + Ghost - Shift, TextPos.y), IM_COL32(0, 220, 255, Glitch ? 220 : 90), "HELLO WORLD");
    Draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), TextPos, Text, "HELLO WORLD");
    ImGui::PopFont();

    const float Underline = TextPos.y + TextSize.y + Scaled(4.F);
    Draw->AddLine(ImVec2(TextPos.x, Underline), ImVec2(TextPos.x + TextSize.x, Underline), Glitch ? Magenta : Cyan, 1.F);

    if (std::fmod(m_Time, 1.1F) < 0.55F)
    {
        Draw->AddRectFilled(ImVec2(TextPos.x + TextSize.x + Scaled(6.F), TextPos.y + Scaled(6.F)),
                            ImVec2(TextPos.x + TextSize.x + Scaled(14.F), TextPos.y + TextSize.y - Scaled(2.F)),
                            Cyan);
    }
}

void Template::HelloWindow::DrawBody()
{
    ImDrawList* const Draw  = ImGui::GetWindowDrawList();
    const float       Width = ImGui::GetWindowSize().x;

    Draw->AddText(At(18.F, 94.F), TextDim, "// SIGNAL ACQUIRED");
    Draw->AddText(At(18.F, 112.F), Text, m_PingTime > 0.F ? "PING ECHO RECEIVED" : "SYSTEM ONLINE");

    // Animated neon progress line.
    const float  Progress = 0.5F + 0.5F * std::sin(m_Time * 1.7F);
    const ImVec2 BarMin   = At(18.F, 138.F);
    const ImVec2 BarMax(ImGui::GetWindowPos().x + Width - Scaled(18.F), BarMin.y + Scaled(4.F));
    const float  Head     = BarMin.x + (BarMax.x - BarMin.x) * Progress;
    const float  Knob     = Scaled(2.F);
    Draw->AddRectFilled(BarMin, BarMax, IM_COL32(0, 40, 55, 200));
    Draw->AddRectFilled(BarMin, ImVec2(Head, BarMax.y), Cyan);
    Draw->AddRectFilled(ImVec2(Head - Knob, BarMin.y - Knob), ImVec2(Head + Knob, BarMax.y + Knob), Magenta);

    ImGui::SetCursorScreenPos(At(18.F, 154.F));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 60, 80, 220));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 30, 160, 200));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 235, 255, 230));
    if (ImGui::Button("PING", ImVec2(Scaled(84.F), Scaled(22.F))))
    {
        m_PingTime = 0.6F;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35F, 0.55F, 0.65F, 1.F));
    ImGui::TextUnformatted("single native window");
    ImGui::PopStyleColor();
}
