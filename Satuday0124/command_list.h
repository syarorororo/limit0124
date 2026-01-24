#pragma once
#include "device.h"
#include "command_allocator.h"

class CommandList final {
public:

    CommandList() = default;

    ~CommandList() = default;

    [[nodiscard]] bool create(const CommandAllocator& commandAllocator) noexcept;

    void reset(const CommandAllocator& commandAllocator) noexcept;

    [[nodiscard]] ID3D12GraphicsCommandList* get() const noexcept;
private:
    Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList> commandList_{};
};