#pragma once
#include "device.h"
#include "swap_chain.h"
#include "descriptor_heap.h"
#include <vector>

class RenderTarget final {
public:

    RenderTarget() = default;

    ~RenderTarget();

    [[nodiscard]] bool createBackBuffer(const Device& device, const SwapChain& swapChain, const DescriptorHeap& heap) noexcept;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const Device& device, const DescriptorHeap& heap, UINT index) const noexcept;

    [[nodiscard]] ID3D12Resource* get(uint32_t index) const noexcept;

private:
    std::vector<ID3D12Resource*> renderTargets_;
};