#pragma once

#include "BorderedBox.h"


class CornersOrnament
{
public:
	CornersOrnament(BorderedBox* rb, float r = 2.0f, DirectX::XMFLOAT2 i = DirectX::XMFLOAT2(20, 20));
	
	inline void SetRatio(float r) { size = DirectX::XMFLOAT2(1, r); }
	inline void ScaleWithWidth(float w) { size = DirectX::XMFLOAT2(w, size.y * w); }
	inline void SetInset(float x, float y) { inset = DirectX::XMFLOAT2(x, y); }
	
	void Draw(float deltaTime);

private:
	BorderedBox* renderBox;
	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT2 inset;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> corner;
};

