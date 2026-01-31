#pragma once
#include"shape.h"

class QuadPolygon final :public Shape {
public:
	QuadPolygon() = default;

	~QuadPolygon() = default;
private:
	[[nodiscard]] virtual bool createVertexBuffer() noexcept override;

	[[nodiscard]] bool createIndexBuffer() noexcept override;
};