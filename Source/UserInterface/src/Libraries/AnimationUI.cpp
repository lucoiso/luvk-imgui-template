/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#include "UserInterface/Libraries/AnimationUI.hpp"
#include <cmath>
#include <imgui.h>

using namespace UserInterface;

Animation::PulseAnimation::PulseAnimation(float Speed)
    : m_Speed(Speed) {}

float Animation::PulseAnimation::GetValue() const
{
    return 0.5F + 0.5F * std::sin(static_cast<float>(ImGui::GetTime()) * m_Speed);
}

Animation::ScanlineAnimation::ScanlineAnimation(const float Frequency)
    : m_Frequency(Frequency) {}

float Animation::ScanlineAnimation::GetValue() const
{
    return std::fmod(static_cast<float>(ImGui::GetTime()) * m_Frequency, 1.0F);
}

Animation::FlickerAnimation::FlickerAnimation(const float Frequency)
    : m_Frequency(Frequency) {}

float Animation::FlickerAnimation::GetValue() const
{
    const float Time = static_cast<float>(ImGui::GetTime());
    return (std::sin(Time * m_Frequency) > 0.0F) ? 1.0F : 0.2F;
}
