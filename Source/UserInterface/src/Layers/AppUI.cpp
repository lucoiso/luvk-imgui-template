// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Layers/AppUI.hpp"
#include <imgui.h>
#include "UserInterface/Libraries/WindowUI.hpp"

using namespace UserInterface;

void AppUI::Draw()
{
    Window::Overlay([]
                    {
                        ImGui::TextColored(ImVec4(0.35F, 0.90F, 0.90F, 1.0F), "HELLO");
                    },
                    []
                    {
                        ImGui::TextColored(ImVec4(0.85F, 0.20F, 0.22F, 1.0F), "WORLD");
                    },
                    []
                    {
                        ImGui::TextColored(ImVec4(0.95F, 0.85F, 0.10F, 1.0F), "E$");
                        ImGui::SameLine(0.0F, 10.0F);
                        ImGui::Text("2025");
                    });
}