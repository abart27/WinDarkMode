> [!WARNING]
> This library is still in its alpha stage.
> While not lacking functionality required for most apps, it's still missing some polish.
> 
> Check back later!

# WinDarkMode

Single-header Win32 C++ dark mode library with a simple API surface based on [win32-darkmode](https://github.com/ysc3839/win32-darkmode).

## Why?

This library is loosely based on win32-darkmode, which isn't easily embeddable and a nightmare to work with.

WinDarkMode is single-file and also one-ups win32-darkmode in various aspects, notably by providing Windows 11 support and automatic theme switching.

## Quickstart

1. Download `WinDarkMode.h` and drop into your project
2. Include it via `#include "WinDarkMode.h"`
3. Make sure to link `UxTheme.lib`, `Comdlg32.lib`, `Dwmapi.lib`
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
            break;
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return TRUE;
}

int CALLBACK WinMain(const HINSTANCE hInstance, HINSTANCE, LPSTR, const int nShowCmd)
{
    WinDarkMode::init();
    RegisterClassEx...
    CreateWindow...
}
```

## Attaching to dialogs

WinDarkMode supports attaching to both top-level windows and dialogs.

To attach to a dialog, you still call `WinDarkMode::attach(hwnd);`, but preferrably on `WM_INITDIALOG`. 

`WM_CREATE` should work too, but is less common.

## Milestones

For those who are interested, here are some feature milestones:

- Control and library support
  - [ ] Animation
  - [x] Button
  - [x] ComboBox
  - [x] ComboBoxEx
  - [ ] Date and Time Picker
  - [x] Edit
  - [ ] Flat Scroll Bar
  - [x] Header Control
  - [ ] Hot Key
  - [ ] Image List
  - [ ] IP Address Control
  - [x] List Box
  - [x] List View
  - [ ] Month Calendar
  - [ ] Pager
  - [ ] Progress Bar
  - [x] Property Sheet
  - [ ] Rebar
  - [ ] Rich Edit
  - [x] Scroll Bar
  - [x] Static Control
  - [x] Status Bar
  - [ ] SysLink
  - [x] Tab
  - [ ] Task Dialog
  - [ ] Toolbar
  - [x] Tooltip
  - [x] Trackbar
  - [x] Tree View
  - [x] Up-Down Control
  - [x] Menu Bar
  - [x] Menu Item
- [x] Automatically react to theme change
- Allow specifying dark mode syncing behavior: system/force light/force dark
