#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "Material.h"
#include "Transform.h"
#include <memory>


class CircleShape
{
public:
	CircleShape(std::shared_ptr<Material> material);
	void Draw();
	Transform* GetTransform();

private:
	unsigned int vertexCount = 38; unsigned int indexCount = 108;

	Vertex vertices[38] = {};

	unsigned int indices[108] = {};
	std::shared_ptr<Material> material;


	Transform transform;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	void CreateVerticesIndices();
	void CreateVertexBuffer();
	void CreateIndexBuffer();

};
