// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/EffectUI.hpp"
#include <cmath>

using namespace UserInterface;

Effect::Unfocus::Unfocus(const float AnimationSpeed)
    : m_Speed(AnimationSpeed)
{
    const float Time(static_cast<float>(ImGui::GetTime()) * m_Speed);
    const float Alpha(0.4F + 0.6F * std::sin(Time));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Alpha);
}

Effect::Unfocus::~Unfocus()
{
    ImGui::PopStyleVar();
}

Effect::Scanline::Scanline(const ImColor& Color, const float Density, const float Speed, const float Thickness)
    : m_Color(Color),
      m_Density(Density),
      m_Speed(Speed),
      m_Thickness(Thickness)
{
    ImGui::BeginGroup();
}

Effect::Scanline::~Scanline()
{
    ImGui::EndGroup();

    const ImVec2 Min = ImGui::GetItemRectMin();
    const ImVec2 Max = ImGui::GetItemRectMax();
    const ImVec2 Size(Max.x - Min.x, Max.y - Min.y);

    if (Size.x <= 0.0F || Size.y <= 0.0F)
    {
        return;
    }

    ImDrawList* DL = ImGui::GetForegroundDrawList();

    const float Time   = static_cast<float>(ImGui::GetTime());
    const float Offset = std::fmod(Time * m_Speed, m_Density);

    const float Flicker = 0.7F + 0.3F * std::sin(Time * 25.0F);

    ImVec4 ColorVec        = m_Color.Value;
    ColorVec.w             *= Flicker;
    const ImU32 FinalColor = ImGui::ColorConvertFloat4ToU32(ColorVec);

    DL->PushClipRect(Min, Max, true);

    for (float It = Offset; It < Size.y; It += m_Density)
    {
        DL->AddLine(ImVec2(Min.x, Min.y + It),
                    ImVec2(Max.x, Min.y + It),
                    FinalColor,
                    m_Thickness);
    }

    DL->PopClipRect();
}

Effect::Glitch::Glitch(const float Intensity)
    : m_Intensity(Intensity)
{
    const float Time = static_cast<float>(ImGui::GetTime());

    const float OffsetX = std::sin(Time * 50.0F) * std::cos(Time * 20.0F) * m_Intensity;
    const float OffsetY = std::cos(Time * 30.0F) * m_Intensity;

    if (std::fmod(Time, 1.0F) > 0.95F)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + OffsetX, ImGui::GetCursorPosY() + OffsetY));
    }
}

Effect::Glow::Glow(const ImColor& Color, const float Radius)
    : m_Color(Color),
      m_Radius(Radius)
{
    ImGui::BeginGroup();
}

Effect::Glow::~Glow()
{
    ImGui::EndGroup();

    const ImVec2 Min = ImGui::GetItemRectMin();
    const ImVec2 Max = ImGui::GetItemRectMax();
    ImDrawList*  DL  = ImGui::GetWindowDrawList();

    ImColor ShadowColor = m_Color;
    for (float It = 1.0F; It <= m_Radius; It += 1.0F)
    {
        ShadowColor.Value.w = (m_Radius - It) / (m_Radius * 10.0F);
        DL->AddRect(ImVec2(Min.x - It, Min.y - It), ImVec2(Max.x + It, Max.y + It), ShadowColor, 0.0F, 0, 1.5F);
    }
}
