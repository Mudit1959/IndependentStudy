#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "Material.h"
#include "Transform.h"
#include "BufferStructs.h"
#include "Graphics.h"
#include "Kinds.h"
#include <memory>


class RectangleEntity
{
public:
	RectangleEntity(std::shared_ptr<Material> material, int kind);
	int GetKind();
	void DrawRect(unsigned int screenWidth, unsigned int screenHeight);
	void DrawCircle(unsigned int screenWidth, unsigned int screenHeight, float radius);
	Transform* GetTransform();

	DirectX::XMFLOAT4 GetColor();
	void SetColor(float r, float g, float b, float a);
	void SetColor(DirectX::XMFLOAT4 c);

	DirectX::XMFLOAT2 GetTranslateXY();
	void SetTranslateXY(float x, float y);
	void SetTranslateXY(DirectX::XMFLOAT2 xy);

private:

	Vertex vertices[4] = {
		{DirectX::XMFLOAT3(+1.0f, +1.0f, +0.0f), Red },
		{DirectX::XMFLOAT3(+1.0f, -1.0f, +0.0f), Blue},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, +0.0f), Green},
		{DirectX::XMFLOAT3(-1.0f, +1.0f, +0.0f), Green}
	};

	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	int kind = 0;
	std::shared_ptr<Material> material;

	DirectX::XMFLOAT4 color; // The color of the object passed into vertex shader
	DirectX::XMFLOAT2 translateXY;
	

	Transform transform;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	unsigned int vertexCount = 4; unsigned int indexCount = 6;
};
