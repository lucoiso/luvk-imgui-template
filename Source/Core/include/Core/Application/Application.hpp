// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include "Core/Application/ApplicationBase.hpp"

namespace Core
{
    class Camera;
    class Cube;
    class Triangle;
    class Pixel;
    class ImGuiLayer;

    class Application : public ApplicationBase
    {
        std::unique_ptr<ImGuiLayer> m_ImGuiLayer;

        explicit Application(std::uint32_t Width, std::uint32_t Height);
        ~Application() override;

    public:
        Application() = delete;
        static Application& GetInstance();

    private:
        void RegisterInputBindings();
    };
}
