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

	const auto size = (sizeof(bufferSize) + 255) & ~255;

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	const auto res = Device::instance().get()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constantBuffe_));
	if (FAILED(res)) {
		assert(false && "コンスタントバッファの作成に失敗");
		return false;
	}

	const auto descriptorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType_);
	if (!descriptorIndex.has_value()) {
		assert(false && "コンスタントバッファのディスクリプタ確保に失敗");
		return false;
	}

	descriptorIndex_ = descriptorIndex.value();

	auto heap = DescriptorHeapContainer::instance().get(heapType_);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constantBuffe_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	UINT cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr += descriptorIndex_ * cbvDescriptorSize;

	Device::instance().get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

	gpuHandle_ = heap->GetGPUDescriptorHandleForHeapStart();

	gpuHandle_.ptr += descriptorIndex_ * cbvDescriptorSize;

	return true;
}

[[nodiscard]] ID3D12Resource* ConstantBuffer::constantBuffer() const noexcept {
	assert(constantBuffe_ && "コンスタントバッファが未作成");
	return constantBuffe_.Get();
}

[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE ConstantBuffer::getGpuDescriptorHandle() const noexcept {
	assert(constantBuffe_ && "コンスタントバッファが未作成");
	return gpuHandle_;
}