// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Layers/ImGuiLayer.hpp"
#include "UserInterface/Layers/AppUI.hpp"
#include "UserInterface/Libraries/ThemeUI.hpp"

using namespace UserInterface;

void ImGuiLayer::Draw()
{
    ImGuiLayerBase::Draw();
    AppUI::Draw();
}

void ImGuiLayer::PushStyle() const
{
    Theme::PushDefaultTheme();
}
