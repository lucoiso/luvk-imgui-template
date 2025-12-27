// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <imgui.h>
#include "UserInterface/Libraries/ThemeUI.hpp"

namespace UserInterface::Data
{
    enum class TextType
    {
        Title,
        Subtitle,
        Content,
        Footer
    };

    USERINTERFACE_API void Text(const char* Content, TextType Category, const ImColor& Color = Theme::Colors::TextWhite, bool Glow = false);
    USERINTERFACE_API void KeyValue(const char* Key, const char* Value, const ImColor& ValueColor = ImColor(255, 255, 255));
}
