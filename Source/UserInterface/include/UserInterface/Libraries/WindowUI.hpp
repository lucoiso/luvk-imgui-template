/*
 * Author: Lucas Vilas-Boas
 * Year: 2025
 * Repo: https://github.com/lucoiso/luvk-imgui-template
 */

#pragma once

#include <functional>
#include <vector>

namespace UserInterface::Window
{
    class USERINTERFACE_API Overlay
    {
        bool*                              m_State;
        const char*                        m_Name;
        std::vector<std::function<void()>> m_Content;

    public:
        explicit Overlay(const char* Name, bool* State);
        ~Overlay();

        void AddContent(std::function<void()>&& Function);
    };
}
