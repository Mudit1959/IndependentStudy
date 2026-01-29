#pragma once
#include "DirectXMath.h"



struct ExtraVertexData // Vertex Constant Buffer Data
{
	DirectX::XMFLOAT4X4 world; 
	DirectX::XMFLOAT4X4 worldInv;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};
