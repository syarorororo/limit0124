#include "command_queue.h"
#include <cassert>



[[nodiscard]] bool CommandQueue::create() noexcept {

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    const auto hr = Device::instance().get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue_));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}

[[nodiscard]] ID3D12CommandQueue* CommandQueue::get() const noexcept {
    if (!commandQueue_) {
        assert(false && "コマンドキューが未作成です");
        return nullptr;
    }
    return commandQueue_.Get();
}