#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "Material.h"
#include "Transform.h"
#include "BufferStructs.h"
#include "Kinds.h"
#include <vector>
#include <memory>

class Line
{
public:

	Line(std::shared_ptr<Material> inMaterial, DirectX::XMFLOAT4 color, unsigned int screenWidth, unsigned int screenHeight);

	DirectX::XMFLOAT4 GetColor();
	void SetColor(float r, float g, float b, float a);
	void SetColor(DirectX::XMFLOAT4 c);

	DirectX::XMFLOAT2 GetTranslateXY();
	void SetTranslateXY(float x, float y);
	void SetTranslateXY(DirectX::XMFLOAT2 xy);

	void AddPoint(float x, float y);
	void CreateBuffers();
	void DrawLine(float uv);

private:
	//Vertex vertices[4] = {
	//	{DirectX::XMFLOAT3(+1.0f, +1.0f, +0.0f), Red },
	//	{DirectX::XMFLOAT3(+1.0f, -1.0f, +0.0f), Blue},
	//	{DirectX::XMFLOAT3(-1.0f, -1.0f, +0.0f), Green},
	//	{DirectX::XMFLOAT3(-1.0f, +1.0f, +0.0f), Green}
	//};

	std::vector<LineVertex> vertices;
	std::vector<unsigned int> indices;

	int kind = GROW_LINE;
	std::shared_ptr<Material> material;

	DirectX::XMFLOAT4 color; // The color of the object passed into vertex shader
	DirectX::XMFLOAT2 translateXY;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	unsigned int screenWidth, screenHeight;

	unsigned int vertexCount = 1; unsigned int indexCount = 0;
};

