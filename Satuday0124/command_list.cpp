#include "command_list.h"
#include <cassert>

CommandList::~CommandList() {

    if (commandList_) {
        commandList_->Release();
        commandList_ = nullptr;
    }
}

[[nodiscard]] bool CommandList::create(const Device& device, const CommandAllocator& commandAllocator) noexcept {

    const auto hr = device.get()->CreateCommandList(0, commandAllocator.getType(), commandAllocator.get(), nullptr, IID_PPV_ARGS(&commandList_));
    if (FAILED(hr)) {
        assert(false && "コマンドリストの作成に失敗しました");
        return false;
    }

    commandList_->Close();
    return true;
}

void CommandList::reset(const CommandAllocator& commandAllocator) noexcept {
    if (!commandList_) {
        assert(false && "コマンドリストが未作成です");
    }

    commandList_->Reset(commandAllocator.get(), nullptr);
}

[[nodiscard]] ID3D12GraphicsCommandList* CommandList::get() const noexcept {
    if (!commandList_) {
        assert(false && "コマンドリストが未作成です");
        return nullptr;
    }
    return commandList_;
}