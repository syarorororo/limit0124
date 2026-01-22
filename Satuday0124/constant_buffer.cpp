#include"constant_buffer.h"
#include<cassert>

namespace {
	constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}

ConstantBuffer::~ConstantBuffer()
{
	DescriptorHeapContainer::instance().releaseDescriptor(heapType_, descriptorIndex_);
}
[[nodiscard]] bool ConstantBuffer::create(UINT bufferSize)noexcept {

}