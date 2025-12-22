// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/overlay-exp

#pragma once

#include <imgui.h>
#include "UserInterface/Module.hpp"

namespace UserInterface::Theme
{
    namespace Colors
    {
        static constexpr ImVec4 Black(0.04F, 0.04F, 0.04F, 1.00F);
        static constexpr ImVec4 DeepRed(0.45F, 0.00F, 0.00F, 1.00F);
        static constexpr ImVec4 BrightRed(0.85F, 0.12F, 0.12F, 1.00F);
        static constexpr ImVec4 Cyan(0.45F, 0.95F, 0.95F, 1.00F);
        static constexpr ImVec4 Yellow(0.95F, 0.85F, 0.15F, 1.00F);
        static constexpr ImVec4 DarkBlue(0.08F, 0.15F, 0.25F, 1.00F);
        static constexpr ImVec4 TextWhite(0.95F, 0.95F, 0.95F, 1.00F);
    }

    USERINTERFACEMODULE_API void PushDefaultTheme();
}
