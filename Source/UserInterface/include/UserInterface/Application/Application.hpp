// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <Core/Application/ApplicationBase.hpp>

namespace UserInterface
{
    class USERINTERFACE_API Application : public Core::ApplicationBase
    {
        static std::shared_ptr<Application> s_Instance;

        explicit Application(std::uint32_t Width, std::uint32_t Height);

    public:
        Application()           = delete;
        ~Application() override = default;

        static std::shared_ptr<Application> GetInstance();

        bool Initialize() override;
        void Shutdown() override;
    };
}
