// Author: Lucas Vilas-Boas
// Year: 2025
// Repo: https://github.com/lucoiso/luvk-imgui-template

#ifndef USERINTERFACEMODULE_H
#define USERINTERFACEMODULE_H

#ifdef __GNUC__
#define USERINTERFACEMODULE_API __attribute__((visibility("default")))
#else
#ifdef BUILD_DLL
#define USERINTERFACEMODULE_API _declspec(dllexport)
#else
#define USERINTERFACEMODULE_API _declspec(dllimport)
#endif // BUILD_DLL
#endif // __GNUC__
#endif // USERINTERFACEMODULE_H
