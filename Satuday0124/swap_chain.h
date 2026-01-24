#pragma once
#include "DXGI.h"
#include "command_queue.h"
#include "window.h"

class SwapChain final {
public:

    SwapChain() = default;

    ~SwapChain() = default;

    [[nodiscard]] bool create(const CommandQueue& commandQueue) noexcept;

    [[nodiscard]] IDXGISwapChain3* get() const noexcept;

    [[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;
private:
    Microsoft::WRL::ComPtr<IDXGISwapChain3>swapChain_{};
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
};