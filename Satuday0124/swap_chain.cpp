#include "swap_chain.h"
#include"DXGI.h"
#include"window.h"
#include <cassert>



[[nodiscard]] bool SwapChain::create(const CommandQueue& commandQueue) noexcept {

    const auto [w, h] = Window::instance().size();
    swapChainDesc_ = {};
    swapChainDesc_.BufferCount = 2;
    swapChainDesc_.Width = w;
    swapChainDesc_.Height = h;
    swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc_.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{}; 
    {
        const auto hr = Device::instance().dxgi().factory()->CreateSwapChainForHwnd(commandQueue.get(), 
            Window::instance().handle(),
            &swapChainDesc_, nullptr, nullptr, tempSwapChain.GetAddressOf());
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    {

        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));

        if (FAILED(hr)) {
            tempSwapChain->Release();
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }
    return true;
}

[[nodiscard]] IDXGISwapChain3* SwapChain::get() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return swapChain_.Get();
}

[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& SwapChain::getDesc() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
    }
    return swapChainDesc_;
}