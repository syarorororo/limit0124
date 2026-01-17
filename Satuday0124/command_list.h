#pragma once
#include "device.h"
#include "command_allocator.h"

class CommandList final {
public:

    CommandList() = default;

    ~CommandList();

    [[nodiscard]] bool create(const Device& device, const CommandAllocator& commandAllocator) noexcept;

    void reset(const CommandAllocator& commandAllocator) noexcept;

    [[nodiscard]] ID3D12GraphicsCommandList* get() const noexcept;
private:
    ID3D12GraphicsCommandList* commandList_{};
};