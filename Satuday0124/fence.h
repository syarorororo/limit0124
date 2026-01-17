#pragma once
#include "device.h"

class Fence final {
public:

    Fence() = default;

    ~Fence();

    [[nodiscard]] bool create(const Device& device) noexcept;

    void wait(UINT64 fenceValue) const noexcept;

    [[nodiscard]] ID3D12Fence* get() const noexcept;
private:
    ID3D12Fence* fence_{};
    HANDLE       waitGpuEvent_{};
};