// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <Core/Application/ApplicationBase.hpp>
#include <Core/UserInterface/ImGuiLayerBase.hpp>
#include <luvk/Modules/Renderer.hpp>
#include "UserInterface/Module.hpp"

namespace UserInterface
{
    class USERINTERFACEMODULE_API Application : public Core::ApplicationBase
    {
        std::unique_ptr<Core::ImGuiLayerBase> m_ImGuiLayer;
        static std::shared_ptr<Application>   s_Instance;

        explicit Application(std::uint32_t Width, std::uint32_t Height);

    public:
        Application() = delete;
        ~Application() override;

        static std::shared_ptr<Application> GetInstance();

        template <typename ImGuiLayerType, typename... Args> requires std::is_base_of_v<Core::ImGuiLayerBase, ImGuiLayerType>
        void RegisterImGuiLayer(Args&&... InArguments)
        {
            const std::shared_ptr<Application> AppInstance = GetInstance();

            AppInstance->m_ImGuiLayer = std::make_unique<ImGuiLayerType>(AppInstance->m_Window,
                                                                         AppInstance->m_Renderer->GetInstance(),
                                                                         AppInstance->m_DeviceModule,
                                                                         AppInstance->m_DescriptorPoolModule,
                                                                         AppInstance->m_SwapChainModule,
                                                                         AppInstance->m_MemoryModule,
                                                                         std::forward<Args>(InArguments)...);
            AppInstance->PostRegisterImGuiLayer();
        }

    private:
        void PostRegisterImGuiLayer() const;
        void RegisterInputBindings();
    };
}
