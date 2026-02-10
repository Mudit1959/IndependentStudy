#include "Camera.h"
#include "Window.h"

Camera::Camera(float newAspectRatio)
{
	fov = DirectX::XM_PIDIV4;
	nearClip = 0.01f;
	farClip = 1000.0f;
	aspectRatio = newAspectRatio;

	view = DirectX::XMFLOAT4X4();
	proj = DirectX::XMFLOAT4X4();
	orthoProj = DirectX::XMFLOAT4X4();

	CalculateProjMatrix();
	CalculateViewMatrix();
	CalculateOrthoProjMatrix();

}

void Camera::Resize(float newAspectRatio) 
{
	aspectRatio = newAspectRatio;
	CalculateProjMatrix();
	CalculateOrthoProjMatrix();
}

void Camera::CalculateProjMatrix()
{
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClip, farClip));
}

void Camera::CalculateOrthoProjMatrix() 
{
	DirectX::XMStoreFloat4x4(&orthoProj, DirectX::XMMatrixOrthographicLH(1.0f, 1.0f, nearClip, farClip)); // 1 - matches window width/height
}

void Camera::CalculateViewMatrix() 
{
	DirectX::XMFLOAT3 position = transform.GetPosition();
	DirectX::XMFLOAT3 forward = transform.GetForward();
	DirectX::XMStoreFloat4x4(&view,
		DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&forward), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
}

void Camera::Update() 
{
	CalculateViewMatrix();
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return view; }
DirectX::XMFLOAT4X4 Camera::GetProjMatrix() { return proj; }
DirectX::XMFLOAT4X4 Camera::GetOrthoProjMatrix() { return orthoProj; }

DirectX::XMFLOAT3 Camera::GetPos() { return transform.GetPosition(); }
Transform* Camera::GetTransform() { return &transform; }