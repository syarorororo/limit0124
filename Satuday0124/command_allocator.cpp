#include "command_allocator.h"
#include <cassert>


[[nodiscard]] bool CommandAllocator::create( const D3D12_COMMAND_LIST_TYPE type) noexcept {

    type_ = type;

    const auto hr = Device::instance().get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
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
    return commandAllocator_.Get();
}

[[nodiscard]] D3D12_COMMAND_LIST_TYPE CommandAllocator::getType() const noexcept {
    if (!commandAllocator_) {
        assert(false && "コマンドリストのタイプが未設定です");
    }
    return type_;
}