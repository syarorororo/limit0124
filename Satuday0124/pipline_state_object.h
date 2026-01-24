#pragma once
#include"device.h"
#include"shader.h"
#include"root_signature.h"

class PiplineStateObject final {
public:
	PiplineStateObject() = default;
	~PiplineStateObject() = default;
	[[nodiscard]] bool create(const Shader& shader, const RootSignature& rootSignature)noexcept;
	[[nodiscard]] ID3D12PipelineState* get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> piplineState_ = {};
};