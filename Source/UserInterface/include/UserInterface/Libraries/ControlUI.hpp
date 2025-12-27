// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <imgui.h>

namespace UserInterface::Control
{
    USERINTERFACE_API bool Button(const char* Label, const ImVec2& Size = ImVec2(0, 0));
    USERINTERFACE_API void Checkbox(const char* Label, bool* State);
    USERINTERFACE_API void Slider(const char* Label, float* Value, float Min, float Max);
    USERINTERFACE_API void ProgressBar(float Fraction, const char* Overlay = nullptr);
    USERINTERFACE_API bool Toggle(const char* Label, bool* State);
    USERINTERFACE_API void ColorPicker(const char* Label, ImColor* Color);
    USERINTERFACE_API bool InputField(const char* Label, char* Buffer, size_t BufferSize);
}
