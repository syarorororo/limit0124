#pragma once
#include "device.h"

class Fence final {
public:

    Fence() = default;

    ~Fence() = default;

    [[nodiscard]] bool create() noexcept;

    void wait(UINT64 fenceValue) const noexcept;

    [[nodiscard]] ID3D12Fence* get() const noexcept;
private:
    Microsoft::WRL::ComPtr<ID3D12Fence>fence_{};
    HANDLE       waitGpuEvent_{};
};