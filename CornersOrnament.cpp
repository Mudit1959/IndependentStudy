#include "CornersOrnament.h"

CornersOrnament::CornersOrnament(BorderedBox* rb, float r, DirectX::XMFLOAT2 i) 
{
	renderBox = rb;
	size = DirectX::XMFLOAT2(1, r);
	inset = i;
}


void CornersOrnament::Draw(float deltaTime) 
{
	renderBox->DrawCornerOrnament(nullptr, inset, size, deltaTime);
}