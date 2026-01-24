#include "render_target.h"
#include <cassert>

namespace {
    constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
}

RenderTarget::~RenderTarget() {
    renderTargets_.clear();
}

[[nodiscard]] bool RenderTarget::createBackBuffer(const SwapChain& swapChain) noexcept {

    const auto& desc = swapChain.getDesc();

    renderTargets_.resize(desc.BufferCount);

    auto heap = DescriptorHeapContainer::instance().get(heapType_);

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();

    for (uint8_t i = 0; i < desc.BufferCount; i++) {
        const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
        if (FAILED(hr)) {
            assert(false && "バックバッファの取得に失敗しました");
            return false;
        }

        Device::instance().get()->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, handle);

        handle.ptr += Device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);
    }
    return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescriptorHandle(UINT index) const noexcept {
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }

    auto heap = DescriptorHeapContainer::instance().get(heapType_);;

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += index * Device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);
    return handle;
}

[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index) const noexcept {
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
        return nullptr;
    }
    return renderTargets_[index].Get();
}