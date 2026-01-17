#include "command_allocator.h"
#include <cassert>

CommandAllocator::~CommandAllocator() {

    if (commandAllocator_) {
        commandAllocator_->Release();
        commandAllocator_ = nullptr;
    }
}

[[nodiscard]] bool CommandAllocator::create(const Device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept {

    type_ = type;

    const auto hr = device.get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
    if (FAILED(hr)) {
        assert(false && "コマンドアロケータの作成に失敗しました");
        return false;
    }
    return true;
}

void CommandAllocator::reset() noexcept {
    if (!commandAllocator_) {
        assert(false && "コマンドアロケータが未作成です");
    }
    commandAllocator_->Reset();
}

[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::get() const noexcept {
    if (!commandAllocator_) {
        assert(false && "コマンドアロケータが未作成です");
        return nullptr;
    }
    return commandAllocator_;
}

[[nodiscard]] D3D12_COMMAND_LIST_TYPE CommandAllocator::getType() const noexcept {
    if (!commandAllocator_) {
        assert(false && "コマンドリストのタイプが未設定です");
    }
    return type_;
}