// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/LayoutUI.hpp"
#include "UserInterface/Libraries/ThemeUI.hpp"

using namespace UserInterface;

void Layout::Separator()
{
    const ImVec2 Pos(ImGui::GetCursorScreenPos());
    const ImVec2 Size(ImGui::GetContentRegionAvail());

    ImColor LineColor(Theme::Colors::BrightRed);
    LineColor.Value.w = 0.4F;

    ImGui::GetWindowDrawList()->AddLine(Pos, ImVec2(Pos.x + Size.x, Pos.y), LineColor, 1.0F);
    ImGui::Dummy(ImVec2(0.0F, 8.0F));
}

Layout::IndentScope::IndentScope(const float IndentSize)
    : m_IndentSize(IndentSize)
{
    ImGui::Indent(m_IndentSize);
}

Layout::IndentScope::~IndentScope()
{
    ImGui::Unindent(m_IndentSize);
}

void Layout::VerticalSpacing(const float Size)
{
    ImGui::Dummy(ImVec2(0.0F, Size));
}
