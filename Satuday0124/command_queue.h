#pragma once
#include "device.h"

class CommandQueue final {
public:

    CommandQueue() = default;

    ~CommandQueue() = default;

    [[nodiscard]] bool create() noexcept;

    [[nodiscard]] ID3D12CommandQueue* get() const noexcept;
private:
    Microsoft::WRL::ComPtr< ID3D12CommandQueue> commandQueue_{};
   
};