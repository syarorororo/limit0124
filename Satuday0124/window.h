#pragma once
#include <Windows.h>
#include <string>

class Window final {
public:

    Window() = default;

    ~Window() = default;

    [[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

    [[nodiscard]] bool messageLoop() const noexcept;

    [[nodiscard]] HWND handle() const noexcept;

    [[nodiscard]] std::pair<int, int> size() const noexcept;
private:
    HWND handle_{};
    int  witdh_{};
    int  height_{};
};