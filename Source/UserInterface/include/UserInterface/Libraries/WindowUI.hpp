// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#pragma once

#include <functional>

namespace UserInterface::Window
{
    USERINTERFACE_API void Overlay(std::function<void()>&& Header, std::function<void()>&& Content, std::function<void()>&& Footer = {});
}