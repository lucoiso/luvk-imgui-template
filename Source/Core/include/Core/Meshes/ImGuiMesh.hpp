// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <imgui.h>
#include <luvk/Types/Mesh.hpp>

namespace luvk
{
    class Device;
    class Memory;
    class DescriptorPool;
    class SwapChain;
    class Material;
}

namespace Core
{
    class CORE_API ImGuiMesh : public luvk::Mesh
    {
        const ImDrawData*       m_ActiveDrawData{nullptr};
        std::vector<ImDrawVert> m_Vertices{};
        std::vector<ImDrawIdx>  m_Indices{};

    public:
        ImGuiMesh() = delete;
        explicit ImGuiMesh(const std::shared_ptr<luvk::Device>&         Device,
                           const std::shared_ptr<luvk::Memory>&         Memory,
                           const std::shared_ptr<luvk::DescriptorPool>& Pool,
                           const std::shared_ptr<luvk::SwapChain>&      Swap);

        explicit ImGuiMesh(const std::shared_ptr<luvk::Device>&   Device,
                           const std::shared_ptr<luvk::Memory>&   Memory,
                           const std::shared_ptr<luvk::Material>& Font);

        void UpdateBuffers(const ImDrawData* DrawData, std::uint32_t CurrentFrame);
        void Render(VkCommandBuffer CommandBuffer, std::uint32_t CurrentFrame) const override;
    };
}
