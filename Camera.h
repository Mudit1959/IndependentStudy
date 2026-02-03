#pragma once
#include "DirectXMath.h"
#include "Transform.h"
class Camera
{
public:
	Camera(float x, float y, float z, float width, float height, float asR);

	void UpdateProjMatrix(float width, float height);
	void UpdateProjMatrix(float asR);
	void UpdateViewMatrix();
	void Update();

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();

private:
	float nearClip, farClip, width, height, aspectRatio;
	DirectX::XMFLOAT4X4 proj, view;
	Transform transform;
};

