#pragma once
#include"device.h"

class RootSignature final {
public:
	RootSignature() = default;

	~RootSignature() = default;
	[[nodiscard]] bool create()noexcept;

	[[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature_{};
};