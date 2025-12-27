/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

namespace UserInterface::Layout
{
    USERINTERFACE_API void Separator();

    class USERINTERFACE_API IndentScope
    {
        float m_IndentSize;

    public:
        explicit IndentScope(float IndentSize = 20.0F);
        ~IndentScope();
    };

    USERINTERFACE_API void VerticalSpacing(float Size = 5.0F);
}
