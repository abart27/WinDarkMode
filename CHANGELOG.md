# next

The preview version of WinDarkMode.

# 2.0.0

This version addresses a thread safety issue.

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
