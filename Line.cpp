#include "Line.h"


Line::Line(std::shared_ptr<Material> inMaterial, DirectX::XMFLOAT4 inColor, unsigned int width, unsigned int height) 
	: material(inMaterial), color(inColor), screenWidth(width), screenHeight(height)
{
	translateXY = DirectX::XMFLOAT2(0, 0);
	vertices.push_back({ DirectX::XMFLOAT3(0, 0, +0.0f), color, 0.0f });
}

void Line::AddPoint(float x, float y)
{
	vertices.push_back({ DirectX::XMFLOAT3(x, y, 0.0f), color, 0.0f }); vertexCount++;
}

void Line::CreateBuffers() 
{
	if (vertexCount == 1) { return; }

	for (int i = 0; i < vertexCount; i++) { vertices[i].Color = color; }

	vertices[0].UV = 0;
	vertices[vertexCount-1].UV = 1;

	if (vertexCount > 2) 
	{
		float part = 1 / vertexCount - 1;

		for (int i = 1; i < vertexCount - 1; i++)
		{
			vertices[i].UV = part;
		}
	}

	if (vertexCount == 2)
	{
		indices.push_back(0);
		indices.push_back(1);
		indexCount = 2;
	}
	else // vertexCount > 2
	{
		for (int i = 0; i < vertexCount-1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indexCount += 2;
		}
	}

	CreateVertexBuffer();
	CreateIndexBuffer();
}

void Line::DrawLine(float uv) 
{
	if (vertexCount == 1) { return; }
}



DirectX::XMFLOAT4 Line::GetColor() { return color; }
void Line::SetColor(float r, float g, float b, float a) { DirectX::XMStoreFloat4(&color, DirectX::XMVectorSet(r, g, b, a)); }
void Line::SetColor(DirectX::XMFLOAT4 c) { color = c; }

DirectX::XMFLOAT2 Line::GetTranslateXY() { return translateXY; }
void Line::SetTranslateXY(float x, float y) { DirectX::XMStoreFloat2(&translateXY, DirectX::XMVectorSet(x, y, 0, 0)); }
void Line::SetTranslateXY(DirectX::XMFLOAT2 xy) { translateXY = xy; }
