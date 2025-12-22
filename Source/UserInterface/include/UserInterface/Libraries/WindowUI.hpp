// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/overlay-exp

#pragma once

#include <functional>
#include "UserInterface/Module.hpp"

namespace UserInterface::Window
{
    USERINTERFACEMODULE_API void Overlay(std::function<void()>&& Header, std::function<void()>&& Content, std::function<void()>&& Footer = {});
}
