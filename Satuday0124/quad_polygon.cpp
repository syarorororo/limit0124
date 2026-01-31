#include"quad_polygon.h"
#include<cassert>

namespace {
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}

[[nodiscard]] bool QuadPolygon::createVertexBuffer() noexcept {
	Vertex vertices[] = {
		{{-0.5f,0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f}},
		{ {0.5f,0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f}},
		{{-0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f}},
		{{0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f}},
	};

	const auto vertexBUfferSize = sizeof(vertices);

	D3D12_HEAP_PROPERTIES heapPropety{};
	heapPropety.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapPropety.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropety.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropety.CreationNodeMask = 1;
	heapPropety.VisibleNodeMask = 1;
