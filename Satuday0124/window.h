#pragma once
#include <Windows.h>
#include <string>

class Window final {
public:
    static Window& instance() noexcept {
        static Window instance;
        return instance;
    }

    [[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

    [[nodiscard]] bool messageLoop() const noexcept;

    [[nodiscard]] HWND handle() const noexcept;

    [[nodiscard]] std::pair<int, int> size() const noexcept;
private:
    Window() = default;
    ~Window() = default;

    Window(const Window& r) = delete;
    Window& operator=(const Window& r) = delete;
    Window(Window&& r) = delete;
    Window& operator=(Window&& r) = delete;
    HWND handle_{};
    int  witdh_{};
    int  height_{};
};