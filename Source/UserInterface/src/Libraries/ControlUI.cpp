// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/ControlUI.hpp"
#include "UserInterface/Libraries/ThemeUI.hpp"

using namespace UserInterface;

bool Control::Button(const char* Label, const ImVec2& Size)
{
    return ImGui::Button(Label, Size);
}

void Control::Checkbox(const char* Label, bool* State)
{
    ImGui::Checkbox(Label, State);
}

void Control::Slider(const char* Label, float* Value, float Min, float Max)
{
    ImGui::SliderFloat(Label, Value, Min, Max);
}

void Control::ProgressBar(const float Fraction, const char* Overlay)
{
    ImGui::ProgressBar(Fraction, ImVec2(-1, 0), Overlay);
}

bool Control::Toggle(const char* Label, bool* State)
{
    const ImVec2 Pos       = ImGui::GetCursorScreenPos();
    const float  Height    = ImGui::GetFrameHeight();
    const float  Width     = Height * 1.55F;
    const float  Radius    = Height * 0.50F;
    const auto   ClickArea = ImVec2(Width, Height);

    const bool Pressed = ImGui::InvisibleButton(Label, ClickArea);
    if (Pressed && State)
    {
        *State = !(*State);
    }

    ImDrawList* DL = ImGui::GetWindowDrawList();

    const ImColor Color = *State
                              ? Theme::Colors::BrightRed
                              : ImColor(50, 50, 50);
    DL->AddRectFilled(Pos, ImVec2(Pos.x + Width, Pos.y + Height), Color, Radius);

    const float CirclePos = *State
                                ? (Pos.x + Width - Radius)
                                : (Pos.x + Radius);
    DL->AddCircleFilled(ImVec2(CirclePos, Pos.y + Radius), Radius - 2.0F, ImColor(255, 255, 255));

    ImGui::SameLine();
    ImGui::Text("%s", Label);

    return Pressed;
}

void Control::ColorPicker(const char* Label, ImColor* Color)
{
    ImGui::ColorEdit4(Label, reinterpret_cast<float*>(&Color->Value), ImGuiColorEditFlags_NoInputs);
}

bool Control::InputField(const char* Label, char* Buffer, const size_t BufferSize)
{
    return ImGui::InputText(Label, Buffer, BufferSize);
}
