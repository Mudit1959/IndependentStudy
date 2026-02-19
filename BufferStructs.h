#pragma once
#include "DirectXMath.h"



struct VertexShaderConstants // Vertex Constant Buffer Data
{
	DirectX::XMFLOAT4X4 world; 
	DirectX::XMFLOAT4X4 worldInvT;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};

struct RectVSConstants 
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 worldInvT;

	
	DirectX::XMFLOAT4 colour;
	DirectX::XMFLOAT2 translateXY;
	DirectX::XMINT2 screenWH;
	
};

struct CirclePSConstants 
{
	float radius;
};

struct RoundedRectConstants 
{
	float roundingRadius;
	DirectX::XMFLOAT2 widthHeight;
};

