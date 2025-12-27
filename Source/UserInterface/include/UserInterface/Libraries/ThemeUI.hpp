/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

#include <imgui.h>

namespace UserInterface::Theme
{
    namespace Colors
    {
        static constexpr ImColor Black(0.04F, 0.04F, 0.04F, 1.00F);
        static constexpr ImColor DeepRed(0.45F, 0.00F, 0.00F, 1.00F);
        static constexpr ImColor BrightRed(0.85F, 0.12F, 0.12F, 1.00F);
        static constexpr ImColor Cyan(0.35F, 0.90F, 0.90F, 1.00F);
        static constexpr ImColor Yellow(0.95F, 0.85F, 0.15F, 1.00F);
        static constexpr ImColor DarkBlue(0.08F, 0.15F, 0.25F, 1.00F);
        static constexpr ImColor TextWhite(0.95F, 0.95F, 0.95F, 1.00F);
    }

    USERINTERFACE_API void PushDefaultTheme();
}
