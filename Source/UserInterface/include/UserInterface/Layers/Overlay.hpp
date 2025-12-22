// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include "Core/UserInterface/ImGuiLayerBase.hpp"
#include "UserInterface/Module.hpp"

namespace UserInterface
{
    class USERINTERFACEMODULE_API Overlay : public Core::ImGuiLayerBase
    {
    public:
        using Core::ImGuiLayerBase::ImGuiLayerBase;
        void Draw() override;
        void PushStyle() const override;
    };
}
