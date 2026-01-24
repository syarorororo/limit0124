#pragma once
#include"device.h"

class Shader final {
public:
	Shader() = default;
	~Shader() =default;
	[[nodiscard]] bool create()noexcept;
	[[nodiscard]] ID3DBlob* vertexShader() const noexcept;
	[[nodiscard]] ID3DBlob* pixelShader()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader_{};
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShader_{};
};