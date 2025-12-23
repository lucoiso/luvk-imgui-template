// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Meshes/ImGuiMesh.hpp"
#include <imgui.h>
#include <luvk/Libraries/ShaderCompiler.hpp>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Resources/Buffer.hpp>
#include <luvk/Resources/DescriptorSet.hpp>
#include <luvk/Resources/Image.hpp>
#include <luvk/Resources/Pipeline.hpp>
#include <luvk/Resources/Sampler.hpp>
#include <luvk/Types/Material.hpp>
#include <luvk/Types/Texture.hpp>

using namespace Core;

constexpr auto g_VertexShader = R"(
struct VSInput {
	[[vk::location(0)]] float2 inPos;
	[[vk::location(1)]] float2 inUV;
	[[vk::location(2)]] float4 inColor;
};

struct PC {
	float2 scale;
	float2 translate;
};
[[vk::push_constant]] PC pc;

[shader("vertex")]
void main(
    VSInput input,
    out float4 Position : SV_POSITION,
    [[vk::location(0)]] out float2 fragUV,
    [[vk::location(1)]] out float4 fragColor)
{
	fragUV = input.inUV;
	fragColor = input.inColor;
	Position = float4(input.inPos * pc.scale + pc.translate, 0.0, 1.0);
}
)";

constexpr auto g_FragmentShader = R"(
struct VSOutput {
	[[vk::location(0)]] float2 fragUV;
	[[vk::location(1)]] float4 fragColor;
};

[[vk::binding(0, 0)]] Sampler2D Font;

[shader("fragment")]
float4 main(VSOutput input) : SV_Target {
	float4 sampledColor = Font.Sample(input.fragUV);
	float4 outColor = input.fragColor * sampledColor;
    return outColor;
}
)";

constexpr VkDescriptorSetLayoutBinding g_FontBinding{0,
                                                     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                     1,
                                                     VK_SHADER_STAGE_FRAGMENT_BIT,
                                                     nullptr};

constexpr std::array g_Bindings{VkVertexInputBindingDescription{0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX}};

constexpr std::array g_Attributes{VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)},
                                  VkVertexInputAttributeDescription{1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)},
                                  VkVertexInputAttributeDescription{2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)}};

constexpr VkPushConstantRange g_PushConstantRange{VK_SHADER_STAGE_VERTEX_BIT,
                                                  0,
                                                  sizeof(float) * 4};

ImGuiMesh::ImGuiMesh(const std::shared_ptr<luvk::Device>&         Device,
                     const std::shared_ptr<luvk::Memory>&         Memory,
                     const std::shared_ptr<luvk::DescriptorPool>& Pool,
                     const std::shared_ptr<luvk::SwapChain>&      Swap)
    : luvk::Mesh(Device, Memory)
{
    const auto FontSet = std::make_shared<luvk::DescriptorSet>(m_Device, Pool, m_Memory);
    FontSet->CreateLayout({.Bindings = std::array{g_FontBinding}});
    FontSet->Allocate();

    const ImGuiIO& IO = ImGui::GetIO();

    unsigned char* Pixels;
    std::int32_t   Width, Height;
    IO.Fonts->GetTexDataAsRGBA32(&Pixels, &Width, &Height);

    const auto Staging = std::make_shared<luvk::Buffer>(Device, Memory);
    Staging->CreateBuffer({.Size = static_cast<VkDeviceSize>(Width * Height * 4),
                           .Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           .MemoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU,
                           .Name = "ImGui Font Staging"});
    Staging->Upload({reinterpret_cast<const std::byte*>(Pixels), static_cast<size_t>(Width * Height * 4)});

    auto FontImage = std::make_shared<luvk::Image>(Device, Memory);
    FontImage->CreateImage({.Extent = {static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1},
                            .Format = VK_FORMAT_R8G8B8A8_UNORM,
                            .Usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                            .Aspect = VK_IMAGE_ASPECT_COLOR_BIT,
                            .MemoryUsage = VMA_MEMORY_USAGE_GPU_ONLY});
    FontImage->Upload(Staging);

    const auto FontSampler = std::make_shared<luvk::Sampler>(Device);
    FontSampler->CreateSampler({.Filter = VK_FILTER_LINEAR, .AddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE});

    const auto FontTexture = std::make_shared<luvk::Texture>(FontImage, FontSampler);
    IO.Fonts->SetTexID(reinterpret_cast<ImTextureID>(FontSet->GetHandle()));
    FontSet->UpdateImage(FontImage->GetView(), FontSampler->GetHandle(), 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

    const auto FontPipeline = std::make_shared<luvk::Pipeline>(m_Device);
    FontPipeline->CreateGraphicsPipeline({.Extent = Swap->GetExtent(),
                                          .ColorFormats = std::array{Swap->GetCreationArguments().Format},
                                          .RenderPass = Swap->GetRenderPass(),
                                          .Subpass = 0,
                                          .VertexShader = luvk::CompileShader(g_VertexShader),
                                          .FragmentShader = luvk::CompileShader(g_FragmentShader),
                                          .SetLayouts = std::array{FontSet->GetLayout()},
                                          .Bindings = g_Bindings,
                                          .Attributes = g_Attributes,
                                          .PushConstants = std::array{g_PushConstantRange},
                                          .Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                          .CullMode = VK_CULL_MODE_NONE,
                                          .EnableDepthOp = false});

    m_Material = std::make_shared<luvk::Material>();
    m_Material->SetPipeline(FontPipeline);
    m_Material->SetDescriptorSet(FontSet);
    m_Material->SetTexture(FontTexture);
}

ImGuiMesh::ImGuiMesh(const std::shared_ptr<luvk::Device>& Device, const std::shared_ptr<luvk::Memory>& Memory, const std::shared_ptr<luvk::Material>& Font)
    : luvk::Mesh(Device, Memory)
{
    m_Material = Font;
}

void ImGuiMesh::UpdateBuffers(const ImDrawData* const DrawData, const std::uint32_t CurrentFrame)
{
    m_ActiveDrawData = nullptr;

    if (DrawData == nullptr || DrawData->TotalVtxCount == 0 || DrawData->TotalIdxCount == 0 || DrawData->CmdListsCount == 0)
    {
        return;
    }

    m_ActiveDrawData = DrawData;

    m_Vertices.resize(DrawData->TotalVtxCount);
    m_Indices.resize(DrawData->TotalIdxCount);

    std::int32_t VtxOffset = 0;
    std::int32_t IdxOffset = 0;
    for (std::int32_t ListIt = 0; ListIt < DrawData->CmdListsCount; ++ListIt)
    {
        const ImDrawList* CmdList = DrawData->CmdLists[ListIt];

        std::memcpy(m_Vertices.data() + VtxOffset,
                    CmdList->VtxBuffer.Data,
                    CmdList->VtxBuffer.Size * sizeof(ImDrawVert));

        std::memcpy(m_Indices.data() + IdxOffset,
                    CmdList->IdxBuffer.Data,
                    CmdList->IdxBuffer.Size * sizeof(ImDrawIdx));

        VtxOffset += CmdList->VtxBuffer.Size;
        IdxOffset += CmdList->IdxBuffer.Size;
    }

    UploadVertices(std::as_bytes(std::span{m_Vertices}), DrawData->TotalVtxCount, CurrentFrame);
    UploadIndices(std::span{m_Indices}, CurrentFrame);

    const std::array ConstantData{2.F / DrawData->DisplaySize.x,
                                  2.F / DrawData->DisplaySize.y,
                                  -1.F - DrawData->DisplayPos.x * (2.F / DrawData->DisplaySize.x),
                                  -1.F - DrawData->DisplayPos.y * (2.F / DrawData->DisplaySize.y)};

    SetPushConstantData(std::as_bytes(std::span{ConstantData}));
}

void ImGuiMesh::Render(const VkCommandBuffer CommandBuffer, const std::uint32_t CurrentFrame) const
{
    if (m_ActiveDrawData == nullptr || std::empty(m_VertexBuffers) || m_VertexBuffers.at(CurrentFrame) == nullptr)
    {
        return;
    }

    const auto FbWidth  = static_cast<std::int32_t>(m_ActiveDrawData->DisplaySize.x * m_ActiveDrawData->FramebufferScale.x);
    const auto FbHeight = static_cast<std::int32_t>(m_ActiveDrawData->DisplaySize.y * m_ActiveDrawData->FramebufferScale.y);

    if (FbWidth <= 0 || FbHeight <= 0)
    {
        return;
    }

    m_Material->Bind(CommandBuffer);

    const auto  Pipeline       = m_Material->GetPipeline();
    const auto& PipelineLayout = Pipeline->GetPipelineLayout();

    if (!std::empty(m_PushConstantData))
    {
        vkCmdPushConstants(CommandBuffer,
                           PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           static_cast<std::uint32_t>(std::size(m_PushConstantData)),
                           std::data(m_PushConstantData));
    }

    const VkBuffer         VtxBuffer = m_VertexBuffers.at(CurrentFrame)->GetHandle();
    constexpr VkDeviceSize Offset    = 0;
    vkCmdBindVertexBuffers(CommandBuffer, 0, 1, &VtxBuffer, &Offset);

    const VkBuffer Idxuffer = m_IndexBuffers.at(CurrentFrame)->GetHandle();
    vkCmdBindIndexBuffer(CommandBuffer, Idxuffer, 0, VK_INDEX_TYPE_UINT16);

    const VkViewport Viewport{0.F, 0.F, static_cast<float>(FbWidth), static_cast<float>(FbHeight), 0.F, 1.F};
    vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

    const ImVec2 ClipOff   = m_ActiveDrawData->DisplayPos;
    const ImVec2 ClipScale = m_ActiveDrawData->FramebufferScale;

    std::int32_t GlobalVtxOffset = 0;
    std::int32_t GlobalIdxOffset = 0;

    for (std::int32_t ListIt = 0; ListIt < m_ActiveDrawData->CmdListsCount; ++ListIt)
    {
        const ImDrawList* CmdList = m_ActiveDrawData->CmdLists[ListIt];
        for (const ImDrawCmd& CmdIt : CmdList->CmdBuffer)
        {
            if (CmdIt.UserCallback)
            {
                CmdIt.UserCallback(CmdList, &CmdIt);
                continue;
            }

            ImVec2 ClipMin((CmdIt.ClipRect.x - ClipOff.x) * ClipScale.x,
                           (CmdIt.ClipRect.y - ClipOff.y) * ClipScale.y);

            ImVec2 ClipMax((CmdIt.ClipRect.z - ClipOff.x) * ClipScale.x,
                           (CmdIt.ClipRect.w - ClipOff.y) * ClipScale.y);

            ClipMin.x = std::max(ClipMin.x, 0.F);
            ClipMin.y = std::max(ClipMin.y, 0.F);
            ClipMax.x = std::min(ClipMax.x, static_cast<float>(FbWidth));
            ClipMax.y = std::min(ClipMax.y, static_cast<float>(FbHeight));

            if (ClipMax.x <= ClipMin.x || ClipMax.y <= ClipMin.y)
            {
                continue;
            }

            VkRect2D Scissor{{static_cast<int32_t>(ClipMin.x), static_cast<int32_t>(ClipMin.y)},
                             {static_cast<uint32_t>(ClipMax.x - ClipMin.x),
                              static_cast<uint32_t>(ClipMax.y - ClipMin.y)}};
            vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

            const auto& DescSet = reinterpret_cast<VkDescriptorSet>(CmdIt.GetTexID());
            vkCmdBindDescriptorSets(CommandBuffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    PipelineLayout,
                                    0,
                                    1,
                                    &DescSet,
                                    0,
                                    nullptr);

            vkCmdDrawIndexed(CommandBuffer,
                             CmdIt.ElemCount,
                             1,
                             static_cast<std::int32_t>(CmdIt.IdxOffset) + GlobalIdxOffset,
                             static_cast<std::int32_t>(CmdIt.VtxOffset) + GlobalVtxOffset,
                             0);
        }

        GlobalIdxOffset += CmdList->IdxBuffer.Size;
        GlobalVtxOffset += CmdList->VtxBuffer.Size;
    }

    const VkRect2D FullScissor{{0, 0}, {static_cast<uint32_t>(FbWidth), static_cast<uint32_t>(FbHeight)}};
    vkCmdSetScissor(CommandBuffer, 0, 1, &FullScissor);
}
