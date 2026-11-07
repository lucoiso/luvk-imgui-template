# luvk-imgui-template

A tiny **pure Dear ImGui** starter built on [luvk](https://github.com/lucoiso/luvk): a small borderless
cyberpunk window that says **HELLO WORLD**. No scene renderer, no triangle, no debug/profiling panels.

- Dear ImGui rendered **by luvk** (bindless pipeline, no official `imgui_impl_vulkan` backend);
- one borderless native window, dragged by the OS through hit-testing (native snapping, no per-frame code);
- the complete cyberpunk style lives in `src/CyberStyle.cpp` — luvk does **not** force a theme;
- custom-drawn window chrome, glitch title, animated status panel and a PING button;
- DPI aware: the window, fonts and the hand-drawn chrome follow the display scale;
- no Vulkan, SDL or file-system code in the template: luvk owns the device, the window and the persisted state;
- every class has a declaration header and a `.cpp`;
- no tests, no vendored shader compiler; the luvk example lives in the submodule and is off by default.

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/luvk-imgui-template
```

`luvk` is already checked out in this working copy. On a fresh clone, initialize the submodule to the
luvk commit used by the template (or point `LUVK_SOURCE_DIR` at a local luvk checkout).

On Windows use the same commands from a Developer Command Prompt (or with MSVC in PATH). Validation layers
are enabled automatically in Debug builds when the Vulkan SDK is installed. `LUVK_APP_MAX_FRAMES=N` renders N
frames and exits (non-zero exit code on validation errors), which is handy for smoke tests.

The build is warning-clean; to make warnings fatal:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLUVK_TEMPLATE_WARNINGS_AS_ERRORS=ON
```

### Presets

```bash
cmake --preset debug              # template
cmake --preset release            # template
cmake --preset debug-example      # template + example
cmake --preset release-example    # template + example
cmake --preset release-portable   # template + example, static MSVC runtime
```

Build with the matching `cmake --build --preset <name>`.

### Building the luvk example

The example lives in the submodule at `luvk/example` and is off by default. Enable it with:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLUVK_BUILD_EXAMPLE=ON
cmake --build build
```

That also enables the runtime Slang compiler and the optional `luvk-scene` module; in that configuration the
`bin/` folder intentionally also contains `luvk-example` (the luvk gallery) and its required Slang DLLs.

| CMake option | Default | Effect |
|--------------|---------|--------|
| `LUVK_TEMPLATE_WARNINGS_AS_ERRORS` | `OFF` | `/WX` on MSVC, `-Werror` elsewhere, for the template and luvk. |
| `LUVK_BUILD_EXAMPLE` | `OFF` | Builds `luvk/example`; also enables Slang. |
| `LUVK_INCLUDE_SLANG_COMPILER` | `OFF` | Builds `luvk-shaders`; implied by `LUVK_BUILD_EXAMPLE`. |
| `LUVK_SOURCE_DIR` | `luvk` | Use another luvk checkout. |

`LUVK_BUILD_APP` (on) and `LUVK_BUILD_TESTS` (off) are set by the template as plain variables, so no option sticks
in the CMake cache after it is turned off.

### Using a local luvk checkout

`luvk` is the submodule at `luvk`. When both repositories are being developed side by side, point
CMake at the checkout instead of updating the submodule:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLUVK_SOURCE_DIR=C:/path/to/luvk
```

## Output

```
build/
  bin/    luvk-imgui-template(.exe)   ← runtime only: executable and required DLLs
  lib/    luvk*.lib                   ← static libraries
  pdb/    *.pdb                       ← debug symbols, never mixed with runtime files
```

`bin/` stays clean. luvk persists the runtime state in the per-user data directory derived from
`ApplicationDesc::Organization` / `AppName` (`%APPDATA%\lucoiso\luvk-imgui-template` on Windows,
`~/Library/Application Support/lucoiso/luvk-imgui-template` on macOS, `$XDG_DATA_HOME/lucoiso/luvk-imgui-template`
on Linux):

* `imgui.ini` — Dear ImGui layout state;
* `pipeline_cache.bin` — Vulkan pipeline cache.

luvk never stages `slangc.exe`, `.ilk` files or other tools in `bin/`. The only DLLs that can appear there are
actual runtime dependencies; this template links luvk, SDL3, Dear ImGui and GLM statically.

## Layout

Template code follows the same split as every luvk module: headers in `include/`, implementation in `src/`.

| File | Purpose |
|------|---------|
| `src/Main.cpp` | Entry point: constructs `TemplateApp` and calls `Run()`. |
| `include/TemplateApp.hpp` + `src/TemplateApp.cpp` | `ApplicationDesc` (borderless, UI-only) and the three hooks it needs. |
| `include/HelloWindow.hpp` + `src/HelloWindow.cpp` | The mini window: custom chrome (drag area reported to the OS), glitch title and body widgets. |
| `include/CyberStyle.hpp` + `src/CyberStyle.cpp` | The neon/cyberpunk ImGui style. Owned by this template, not by luvk. |
| `luvk` | luvk submodule; provides `luvk::app` (core + platform + ImGui backend). |

Inside the submodule the same module contract is kept:

```text
luvk/
  core/      include/ + src/
  platform/  include/ + src/
  imgui/     include/ + src/ (+ shaders/: GLSL embedded in the module)
  scene/     include/ + src/ (+ shaders/; optional, not built by the template)
  app/       include/ + src/ (+ shaders/: GLSL embedded in the module)
  shaders/   include/ + src/ (optional runtime Slang compiler)
  example/   include/ + src/ (the gallery)
  cmake/  tools/
  CMakeLists.txt
```

The luvk tests are intentionally **not** present in this template checkout.

## Style

The style is intentionally **application-owned**. `luvk-imgui` creates the ImGui context, backends and
renderer, but does not call `ImGui::StyleColors*` or apply a theme. Everything visual starts in
`CyberStyle::Apply()`; replace it or call your own style in `OnInit()`. luvk scales the style sizes by the display
scale right after `OnInit()` (disable with `ApplicationDesc::ScaleUiWithDisplay = false`).

## Window and transparency

The native window is borderless and **exactly the size of the ImGui window** (`460x240` logical units), so there
is no native background strip around the UI. `HelloWindow` reports its title bar to the OS through
`luvk::platform::Window::SetHitTest`: dragging it moves the window natively (snapping included, also on Wayland)
and the close button stays clickable. The ImGui window cannot be torn out into a second OS window.

`TemplateApp` keeps the default `ApplicationDesc::Transparent = false`. Vulkan only supports alpha
composition when the surface reports an alpha mode; on drivers that report
`VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR` only (including the AMD RX 7600 driver used during development), a
transparent request would fall back to opaque and log a warning. If you need real per-pixel transparency on
Windows, that requires a DXGI/DirectComposition path, not the Vulkan swapchain alone.

## Requirements

C++23 compiler, CMake 3.28+, Vulkan 1.3. SDL3, GLM and Dear ImGui are fetched by the corresponding luvk
layers (`luvk-platform`, `luvk-imgui`). No Slang is downloaded or linked.
