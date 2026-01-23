#include "descriptor_heap.h"
#include "device.h"
#include"wrl/client.h"
#include <cassert>
class DescriptorHeap final {
public:
    DescriptorHeap() = default;
    ~DescriptorHeap() = default;

    [[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible) noexcept {

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = type;
    heapDesc.NumDescriptors = numDescriptors;
    heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    type_ = type;

    HRESULT hr = Device::instance().get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
    if (FAILED(hr)) {
        assert(false && "ディスクリプタヒープの生成に失敗しました");
        return false;
    }

    maxDescriptorCount_ = numDescriptors;
    for (UINT i = 0;i < maxDescriptorCount_;++i) {
        freeIndices_.push_back(i);
    }
   
    return true;
}
};
DescriptorHeap::~DescriptorHeap() {

    if (heap_) {
        heap_->Release();
        heap_ = nullptr;
    }
}



[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::get() const noexcept {
    if (!heap_) {
        assert(false && "ディスクリプタヒープが未生成です");
    }
    return heap_;
}

[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::getType() const noexcept {
    if (!heap_) {
        assert(false && "ディスクリプタヒープが未生成です");
    }
    return type_;
}