// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "UserInterface/Libraries/ButtonUI.hpp"
#include <imgui.h>
#include <SDL3/SDL_events.h>

using namespace UserInterface;

void Button::CloseApp()
{
    if (ImGui::Button("X", ImVec2(22.0F, 22.0F)))
    {
        SDL_Event QuitEvent{.type = SDL_EVENT_QUIT};
        SDL_PushEvent(&QuitEvent);
    }
}