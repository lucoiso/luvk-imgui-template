// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include <cstdio>
#include <exception>
#include "Core/Application/Application.hpp"

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
        Core::Application& AppInstance = Core::Application::GetInstance();

        do {}
        while (AppInstance.Render());

        return EXIT_SUCCESS;
    }
    catch (const std::exception& Exception)
    {
        std::printf("Error: %s\n", Exception.what());
    }

    return EXIT_FAILURE;
}
