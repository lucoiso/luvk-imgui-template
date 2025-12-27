/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

namespace UserInterface::Animation
{
    class USERINTERFACE_API AnimationBase
    {
    public:
        virtual                     ~AnimationBase() = default;
        [[nodiscard]] virtual float GetValue() const = 0;
    };

    class USERINTERFACE_API PulseAnimation : public AnimationBase
    {
        float m_Speed;

    public:
        explicit            PulseAnimation(float Speed);
        [[nodiscard]] float GetValue() const override;
    };

    class USERINTERFACE_API ScanlineAnimation : public AnimationBase
    {
        float m_Frequency;

    public:
        explicit            ScanlineAnimation(float Frequency);
        [[nodiscard]] float GetValue() const override;
    };

    class USERINTERFACE_API FlickerAnimation : public AnimationBase
    {
        float m_Frequency;

    public:
        explicit            FlickerAnimation(float Frequency);
        [[nodiscard]] float GetValue() const override;
    };
}
