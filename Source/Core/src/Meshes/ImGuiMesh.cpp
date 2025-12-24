// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#include "Core/Meshes/ImGuiMesh.hpp"
#include <cstring>
#include <imgui.h>
#include <luvk/Modules/SwapChain.hpp>
#include <luvk/Resources/Buffer.hpp>
#include <luvk/Resources/DescriptorSet.hpp>
#include <luvk/Resources/Image.hpp>
#include <luvk/Resources/Pipeline.hpp>
#include <luvk/Resources/Sampler.hpp>
#include <luvk/Types/Material.hpp>
#include <luvk/Types/Texture.hpp>

using namespace Core;

constexpr std::uint32_t g_ImGuiVertexShader[] = {0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
                                                 0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
                                                 0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
                                                 0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
                                                 0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
                                                 0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
                                                 0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
                                                 0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
                                                 0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
                                                 0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
                                                 0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
                                                 0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
                                                 0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
                                                 0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
                                                 0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
                                                 0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
                                                 0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
                                                 0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
                                                 0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
                                                 0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
                                                 0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
                                                 0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
                                                 0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
                                                 0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
                                                 0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
                                                 0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
                                                 0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
                                                 0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
                                                 0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
                                                 0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
                                                 0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
                                                 0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
                                                 0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
                                                 0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
                                                 0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
                                                 0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
                                                 0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
                                                 0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
                                                 0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
                                                 0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
                                                 0x0000002d,0x0000002c,0x000100fd,0x00010038};

constexpr std::uint32_t g_ImGuiFragmentShader[] = {0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
                                                   0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
                                                   0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
                                                   0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
                                                   0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
                                                   0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
                                                   0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
                                                   0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
                                                   0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
                                                   0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
                                                   0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
                                                   0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
                                                   0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
                                                   0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
                                                   0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
                                                   0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
                                                   0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
                                                   0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
                                                   0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
                                                   0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
                                                   0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
                                                   0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
                                                   0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
                                                   0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
                                                   0x00010038};

ImGuiMesh::ImGuiMesh(const std::shared_ptr<luvk::Device>&         Device,
                     const std::shared_ptr<luvk::Memory>&         Memory,
                     const std::shared_ptr<luvk::DescriptorPool>& Pool,
                     const std::shared_ptr<luvk::SwapChain>&      Swap)
    : luvk::Mesh(Device, Memory)
{
    constexpr VkDescriptorSetLayoutBinding FontBinding{0,
                                                       VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                       1,
                                                       VK_SHADER_STAGE_FRAGMENT_BIT,
                                                       nullptr};

    constexpr std::array Bindings{VkVertexInputBindingDescription{0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX}};

    constexpr std::array Attributes{VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)},
                                    VkVertexInputAttributeDescription{1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)},
                                    VkVertexInputAttributeDescription{2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)}};

    constexpr VkPushConstantRange PushConstantRange{VK_SHADER_STAGE_VERTEX_BIT,
                                                    0,
                                                    sizeof(float) * 4};

    const auto FontSet = std::make_shared<luvk::DescriptorSet>(m_Device, Pool, m_Memory);
    FontSet->CreateLayout({.Bindings = std::array{FontBinding}});
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
                                          .VertexShader = g_ImGuiVertexShader,
                                          .FragmentShader = g_ImGuiFragmentShader,
                                          .SetLayouts = std::array{FontSet->GetLayout()},
                                          .Bindings = Bindings,
                                          .Attributes = Attributes,
                                          .PushConstants = std::array{PushConstantRange},
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
