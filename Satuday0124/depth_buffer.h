#pragma once
#include"device.h"

class DepthBuffer final {
public:
	DepthBuffer() = default;

	~DepthBuffer();

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12Resource* depthBuffer() const noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource>depthBuffer_{};
	D3D12_CPU_DESCRIPTOR_HANDLE handle_{};
};