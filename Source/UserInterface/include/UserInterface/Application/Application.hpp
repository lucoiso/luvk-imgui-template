// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <Core/Application/ApplicationBase.hpp>
#include <Core/UserInterface/ImGuiLayerBase.hpp>
#include <luvk/Modules/Renderer.hpp>

namespace UserInterface
{
    class USERINTERFACE_API Application : public Core::ApplicationBase
    {
        std::unique_ptr<Core::ImGuiLayerBase> m_ImGuiLayer;
        static std::shared_ptr<Application>   s_Instance;

        explicit Application(std::uint32_t Width, std::uint32_t Height);

    public:
        Application()           = delete;
        ~Application() override = default;

        static std::shared_ptr<Application> GetInstance();

        bool Initialize() override;
        void Shutdown() override;

        template <typename ImGuiLayerType, typename... Args> requires std::is_base_of_v<Core::ImGuiLayerBase, ImGuiLayerType>
        void RegisterImGuiLayer(Args&&... InArguments)
        {
            m_ImGuiLayer = std::make_unique<ImGuiLayerType>(m_Window,
                                                            m_Renderer->GetInstance(),
                                                            m_DeviceModule,
                                                            m_DescriptorPoolModule,
                                                            m_SwapChainModule,
                                                            m_MemoryModule,
                                                            std::forward<Args>(InArguments)...);
            PostRegisterImGuiLayer();
        }

    private:
        void PostRegisterImGuiLayer() const;
        void RegisterInputBindings();
    };
}
