/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

#include <imgui.h>

namespace UserInterface::Effect
{
    class USERINTERFACE_API Unfocus
    {
        float m_Speed;

    public:
        explicit Unfocus(float AnimationSpeed = 2.0F);
        ~Unfocus();
    };

    class USERINTERFACE_API Scanline
    {
        ImColor m_Color;
        float   m_Density;
        float   m_Speed;
        float   m_Thickness;

    public:
        explicit Scanline(const ImColor& Color = ImColor(0, 0, 0, 255), float Density = 4.0F, float Speed = 10.0F, float Thickness = 1.0F);
        ~Scanline();
    };

    class USERINTERFACE_API Glitch
    {
        float m_Intensity;

    public:
        explicit Glitch(float Intensity = 2.0F);
    };

    class USERINTERFACE_API Glow
    {
        ImColor m_Color;
        float   m_Radius;

    public:
        explicit Glow(const ImColor& Color, float Radius = 5.0F);
        ~Glow();
    };
}
