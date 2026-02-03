#include "Camera.h"
Camera::Camera(float x, float y, float z, float w, float h, float asR)
{
	transform.SetPosition(x, y, z);
	nearClip = 0.01f;
	farClip = 400.0f;
	UpdateProjMatrix(asR);
}

void Camera::UpdateProjMatrix(float w, float h)
{
	width = w;
	height = h;

	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixOrthographicLH(width, height, nearClip, farClip));
}

void Camera::UpdateProjMatrix(float asR) 
{
	aspectRatio = asR;

	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, aspectRatio, nearClip, farClip));
}

void Camera::UpdateViewMatrix() 
{
	DirectX::XMFLOAT3 position = transform.GetPosition();
	DirectX::XMFLOAT3 forward = transform.GetForward();
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f,1.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&forward), DirectX::XMLoadFloat3(&up)));
}

void Camera::Update() { UpdateViewMatrix(); }

DirectX::XMFLOAT3 Camera::GetPosition() { return transform.GetPosition(); }
DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return view; }
DirectX::XMFLOAT4X4 Camera::GetProjMatrix() { return proj; }