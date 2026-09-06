> [!CAUTION]
> WinDarkMode is ready to use, but expect breaking changes until further notice.

# WinDarkMode

Single-header Win32 C++ dark mode library with a simple but flexible API surface.

Based on [win32-darkmode](https://github.com/ysc3839/win32-darkmode) with various enhancements, see the Features section below.

## Features

- Single-file
- Windows 11 support
- Automatic theme switching
- Compatible with MSVC, Clang, GCC
- Works under MinGW
- Contains a demo project

<details>
<summary>Control Support</summary>

| Control | Control |
| --- | --- |
| ⬜ Animation | ✅ Property Sheet |
| ✅ Button | ⬜ Rebar |
| ✅ ComboBox | ⬜ Rich Edit |
| ✅ ComboBoxEx | ✅ Scroll Bar |
| ⬜ Date and Time Picker | ✅ Static Control |
| ✅ Edit | ✅ Status Bar |
| ⬜ Flat Scroll Bar | ⬜ SysLink |
| ✅ Header Control | ✅ Tab |
| ⬜ Hot Key | ⬜ Task Dialog |
| ⬜ Image List | ⬜ Toolbar |
| ⬜ IP Address Control | ✅ Tooltip |
| ✅ List Box | ✅ Trackbar |
| ✅ List View | ✅ Tree View |
| ⬜ Month Calendar | ✅ Up-Down Control |
| ⬜ Pager | ✅ Menu Bar |
| ⬜ Progress Bar | ✅ Menu Item |

</details>

## Quickstart

1. Download `WinDarkMode.h` and drop into your project
2. Include it via `#include "WinDarkMode.h"`
3. Link `uxtheme.lib`, `comdlg32.lib`, `dwmapi.lib` in your parent project
4. Initialize it in WinMain `WinDarkMode::init()`
5. Attach it to a window `WinDarkMode::attach(hwnd)`

```cpp
#include "WinDarkMode.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
        case WM_CREATE:
            WinDarkMode::attach(hwnd);

            // Additional attach options:
            // WinDarkMode::attach(hwnd, {.is_dialog = true});
            // WinDarkMode::attach(hwnd, {.exclude = {GetDlgItem(hwnd, IDC_EXCLUDED)}});
            break;
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return TRUE;
}

int CALLBACK WinMain(const HINSTANCE hInstance, HINSTANCE, LPSTR, const int nShowCmd)
{
    WinDarkMode::init();
    // RegisterClassEx...
    // CreateWindow...
}
```

## Dialog Support

WinDarkMode supports attaching to both top-level windows and dialogs out of the box.
