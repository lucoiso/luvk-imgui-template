// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include <cstdio>
#include <exception>
#include "UserInterface/Application/Application.hpp"
#include "UserInterface/Layers/Overlay.hpp"

#ifdef main
#undef main
#endif

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
std::int32_t main()
#endif
{
    try
    {
        if (const auto AppInstance = UserInterface::Application::GetInstance())
        {
            AppInstance->RegisterImGuiLayer<UserInterface::Overlay>();

            do {}
            while (AppInstance->Render());

            return EXIT_SUCCESS;
        }
    }
    catch (const std::exception& Exception)
    {
        std::printf("Error: %s\n", Exception.what());
    }

    return EXIT_FAILURE;
}
