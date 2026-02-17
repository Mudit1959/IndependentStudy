#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "Material.h"
#include "Transform.h"
#include <memory>


class RectangleShape
{
public:
	RectangleShape(std::shared_ptr<Material> material);
	void Draw();
	Transform* GetTransform();

private:

	Vertex vertices[4] = {
		{DirectX::XMFLOAT3(+1.0f, +1.0f, +0.0f), Red },
		{DirectX::XMFLOAT3(+1.0f, -1.0f, +0.0f), Blue},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, +0.0f), Green},
		{DirectX::XMFLOAT3(-1.0f, +1.0f, +0.0f), Green}
	};

	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	std::shared_ptr<Material> material;


	Transform transform;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	unsigned int vertexCount = 4; unsigned int indexCount = 6;
};
