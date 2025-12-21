// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include "Core/UserInterface/ImGuiLayerBase.hpp"

namespace Core
{
    class ImGuiLayer : public ImGuiLayerBase
    {
    public:
        using ImGuiLayerBase::ImGuiLayerBase;
        void Draw() override;
        void PushStyle() const override;
    };
}
