#pragma once
#include "DirectXMath.h"
#include "Transform.h"

class Camera
{
public:
	Camera(float aspectRatio);
	
	void Resize(float newAspectRatio);
	void CalculateProjMatrix();
	void CalculateViewMatrix();

	void Update();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();
	DirectX::XMFLOAT3 GetPos();
	Transform* GetTransform();

private:
	DirectX::XMFLOAT4X4 view, proj;
	float fov, aspectRatio, nearClip, farClip; 
	Transform transform;

};

