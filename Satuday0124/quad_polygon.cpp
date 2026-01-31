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

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = vertexBUfferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto res = Device::instance().get()->CreateCommittedResource(
		&heapPropety, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertexBuffer_));
	if (FAILED(res)) {
		assert(false && "頂点バッファの作成に失敗しましタ");
		return false;
	}

	Vertex* data{};

	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗しました");
		return false;
	}
	memcpy_s(data, vertexBUfferSize, vertices, vertexBUfferSize);

	vertexBuffer_->Unmap(0, nullptr);

	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = vertexBUfferSize;
	vertexBufferView_.StrideInBytes = sizeof(Vertex);

	topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	return true;
}

[[nodiscard]] bool QuadPolygon::createIndexBuffer() noexcept {
	uint16_t indices[] = { 0,1,2,3 };

	const auto indexBufferSize = sizeof(indices);

	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = indexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto res = Device::instance().get()->CreateCommittedResource(
		&heapProperty, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&indexBuffer_));
	if (FAILED(res)) {
		assert(false && "インデックスバッファの作成に失敗しました");
		return false;
	}

	uint16_t* data{};
	res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "インデックスバッファのマップに失敗しました");
		return false;
	}

	memcpy_s(data, indexBufferSize, indices, indexBufferSize);

	indexBuffer_->Unmap(0, nullptr);

	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

	indexCount_ = _countof(indices);

	return true;
}