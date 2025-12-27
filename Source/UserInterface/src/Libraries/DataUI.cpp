// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/DataUI.hpp"

using namespace UserInterface;

void Data::Text(const char* Content, const TextType Category, const ImColor& Color, bool Glow)
{
    float Scale = 1.0F;
    switch (Category)
    {
    case TextType::Title: Scale = 1.3F;
        break;
    case TextType::Subtitle: Scale = 1.1F;
        break;
    case TextType::Footer: Scale = 0.85F;
        break;
    default: break;
    }

    ImGui::SetWindowFontScale(Scale);

    if (Glow)
    {
        const ImVec2 Pos       = ImGui::GetCursorScreenPos();
        ImDrawList*  DL        = ImGui::GetWindowDrawList();
        ImColor      GlowColor = Color;
        GlowColor.Value.w      = 0.3F;

        for (float It = 1.0F; It <= 3.0F; It += 1.0F)
        {
            DL->AddText(ImVec2(Pos.x + It, Pos.y), GlowColor, Content);
            DL->AddText(ImVec2(Pos.x - It, Pos.y), GlowColor, Content);
        }
    }

    ImGui::TextColored(Color, "%s", Content);
    ImGui::SetWindowFontScale(1.0F);
}

void Data::KeyValue(const char* Key, const char* Value, const ImColor& ValueColor)
{
    ImGui::Text("%s:", Key);
    ImGui::SameLine();
    ImGui::TextColored(ValueColor, "%s", Value);
}
