# next

The preview version of WinDarkMode.

## 🐞 Bugfixes

- Fixed issues with owner-drawn statusbars.

## ⚡ Performance

- Optimized dark mode statusbar drawing

# 2.0.0

This version addresses thread safety and Unicode issues.

## 🐞 Bugfixes

- Fixed various Unicode issues

## 🛑 Breaking Changes

### `WinDarkMode::light_theme_data`, `WinDarkMode::dark_theme_data`, and `WinDarkMode::theme_data` privated

These fields are now part of the internal API - do not use them.

They were removed because accidentally reading them off-thread was too easy.

Use `WinDarkMode::theme_data()` and `WinDarkMode::theme_data(WinDarkMode::Theme)` instead. You won't need these often anyway.

# 1.0.0

The initial stable release of WinDarkMode.

Features:

- Single-file
- Compatible with Windows 11 25H2
- Automatic theme switching
- Compatible with MSVC, Clang, GCC
- Works under MinGW
