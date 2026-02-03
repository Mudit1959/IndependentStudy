#include "Transform.h"

Transform::Transform() 
{
	SetPosition(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
	SetRotation(0, 0, 0);

	DirectX::XMMATRIX initialMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&world, initialMatrix);
	DirectX::XMStoreFloat4x4(&worldInvT, DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(initialMatrix)));

	DirectX::XMStoreFloat3(&relUp, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&relRight, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&relForward, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));

	edited = 0;

	CalculateWorldMatrix();
}

// Setters
void Transform::SetPosition(DirectX::XMFLOAT3 inPos) { position = inPos; edited++; }
void Transform::SetPosition(float x, float y, float z) { position = DirectX::XMFLOAT3(x,y,z); edited++; }
void Transform::SetScale(DirectX::XMFLOAT3 inScale) { scale = inScale; edited++; }
void Transform::SetScale(float x, float y, float z) { scale = DirectX::XMFLOAT3(x, y, z); edited++; }
void Transform::SetRotation(DirectX::XMFLOAT3 inRot) { rotation = inRot; edited++; }
void Transform::SetRotation(float x, float y, float z) { rotation = DirectX::XMFLOAT3(x, y, z); edited++; }

// Getters
DirectX::XMFLOAT3 Transform::GetPosition() { return position; }
DirectX::XMFLOAT3 Transform::GetPitchYawRoll() { return rotation; }
DirectX::XMFLOAT3 Transform::GetScale() { return scale; }
DirectX::XMFLOAT3 Transform::GetForward() { return relUp; }
DirectX::XMFLOAT3 Transform::GetRight() { return relForward; }
DirectX::XMFLOAT3 Transform::GetUp() { return relRight; }

// Movements
void Transform::MoveAbsolute(DirectX::XMFLOAT3 move) 
{
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorSet(move.x, move.y, move.z, 1.0f)));
	edited++;
}
void Transform::MoveAbsolute(float x, float y, float z) 
{
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorSet(x, y, z, 1.0f)));
	edited++;
}
void Transform::Scale(DirectX::XMFLOAT3 inScale) 
{
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(inScale.x, inScale.y, inScale.z, 1.0f)));
	edited++;
}
void Transform::Scale(float x, float y, float z) 
{
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(x, y, z, 1.0f)));
	edited++;
}

void Transform::Rotate(DirectX::XMFLOAT3 inRotate) 
{
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorSet(inRotate.x, inRotate.y, inRotate.z, 1.0f)));
	edited++;
}
void Transform::Rotate(float x, float y, float z) 
{
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&rotation), DirectX::XMVectorSet(x, y, z, 1.0f)));
	edited++;
}

// Calculate
void Transform::CalculateWorldMatrix() 
{
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&world, worldMatrix);
	DirectX::XMStoreFloat4x4(&worldInvT, DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(worldMatrix)));
	edited = 0;

}


DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() 
{
	if (edited != 0) { CalculateWorldMatrix(); return world; }
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseMatrix() 
{
	if (edited != 0) { CalculateWorldMatrix(); return worldInvT; }
}