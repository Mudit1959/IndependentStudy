#pragma once
#include "DirectXMath.h"



struct VertexShaderConstants // Vertex Constant Buffer Data
{
	DirectX::XMFLOAT4X4 world; 
	DirectX::XMFLOAT4X4 worldInvT;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};
