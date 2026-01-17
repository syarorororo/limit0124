#pragma once
#include"device.h"
#include"command_list.h"

class TrianglePolygon final {
public:
	TrianglePolygon() = default;
	~TrianglePolygon();

	[[nodiscard]] bool create(const Device& device) noexcept;
	[[nodiscard]] void draw(const CommandList& commandList) noexcept;

private:
	[[nodiscard]] bool createVertexBuffer(const Device& device) noexcept;
	[[nodiscard]] bool createIndexBuffer(const Device& device) noexcept;
private:
	ID3D12Resource* vertexBuffer_{};
	ID3D12Resource* indexBuffer_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferview_ = {};
};