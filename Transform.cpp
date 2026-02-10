#include "Transform.h"
#include "Window.h"

Transform::Transform() 
{
	edited = 0;
	orientEdited = 0;

	DirectX::XMMATRIX initialMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&world, initialMatrix);
	DirectX::XMStoreFloat4x4(&worldInverseT, DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(initialMatrix)));

	SetScale(1.0f, 1.0f, 1.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetPosition(0.0f, 0.0f, 0.0f);

	CalculateOrientation();
	CalculateWorldMatrix();
}

void Transform::SetPosition(DirectX::XMFLOAT3 inPos) { DirectX::XMStoreFloat3(&position, DirectX::XMLoadFloat3(&inPos));  edited++; }
void Transform::SetPosition(float x, float y, float z) { DirectX::XMStoreFloat3(&position, DirectX::XMVectorSet(x, y, z, 0.0f)); edited++; }

void Transform::SetScale(DirectX::XMFLOAT3 inScale) { DirectX::XMStoreFloat3(&scale, DirectX::XMLoadFloat3(&inScale)); edited++; }
void Transform::SetScale(float x, float y, float z) { DirectX::XMStoreFloat3(&scale, DirectX::XMVectorSet(x, y, z, 0.0f)); edited++; }

void Transform::SetRotation(DirectX::XMFLOAT3 inRot) { DirectX::XMStoreFloat3(&rotation, DirectX::XMLoadFloat3(&inRot)); edited++; orientEdited++; }
void Transform::SetRotation(float x, float y, float z) { DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorSet(x, y, z, 0.0f)); edited++; orientEdited++; }


DirectX::XMFLOAT3 Transform::GetPosition() { return position; }
DirectX::XMFLOAT3 Transform::GetRotation() { return rotation; }
DirectX::XMFLOAT3 Transform::GetScale() { return scale; }
DirectX::XMFLOAT3 Transform::GetForward() { if (orientEdited != 0) { CalculateOrientation(); } return relForward; }
DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() 
{
	if (edited != 0) { CalculateWorldMatrix(); } 
	return world;
}
DirectX::XMFLOAT4X4 Transform::GetWorldInverseTMatrix() 
{ 
	if (edited != 0) { CalculateWorldMatrix(); }
	return worldInverseT;
}

void Transform::MoveAbsolute(DirectX::XMFLOAT3 moveBy) { DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&moveBy)));  edited++; }
void Transform::MoveAbsolute(float x, float y, float z) { DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorSet(x, y, z, 0.0f)));  edited++; }

void Transform::MoveRelative(DirectX::XMFLOAT3 moveBy) 
{ 
	DirectX::XMFLOAT3 iRight = DirectX::XMFLOAT3(relRight.x * moveBy.x, relRight.y * moveBy.x, relRight.z * moveBy.x);
	DirectX::XMFLOAT3 iUp = DirectX::XMFLOAT3(relUp.x * moveBy.y, relUp.y * moveBy.y, relUp.z * moveBy.y);
	DirectX::XMFLOAT3 iForward = DirectX::XMFLOAT3(relForward.x * moveBy.z, relForward.y * moveBy.z, relForward.z * moveBy.z);

	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), 
		DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&iRight), 
		DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&iUp), DirectX::XMLoadFloat3(&iForward)))));

	edited++; 
}
void Transform::MoveRelative(float x, float y, float z) 
{
	DirectX::XMFLOAT3 iRight = DirectX::XMFLOAT3(relRight.x * x, relRight.y * x, relRight.z * x);
	DirectX::XMFLOAT3 iUp = DirectX::XMFLOAT3(relUp.x * y, relUp.y * y, relUp.z * y);
	DirectX::XMFLOAT3 iForward = DirectX::XMFLOAT3(relForward.x * z, relForward.y * z, relForward.z * z);

	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position),
		DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&iRight),
			DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&iUp), DirectX::XMLoadFloat3(&iForward)))));
	edited++; 
}

void Transform::Rotate(DirectX::XMFLOAT3 rotateBy) { DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&rotation), DirectX::XMLoadFloat3(&rotateBy)));  CalculateOrientation();  edited++; orientEdited++; }
void Transform::Rotate(float x, float y, float z) { DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&rotation), DirectX::XMVectorSet(x, y, z, 0.0f)));  CalculateOrientation();  edited++; orientEdited++; }

void Transform::Scale(DirectX::XMFLOAT3 scaleBy) { DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMLoadFloat3(&scaleBy)));  edited++; }
void Transform::Scale(float x, float y, float z) { DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(x, y, z, 0.0f)));  edited++; }

void Transform::CalculateWorldMatrix() 
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&world, matrix);
	DirectX::XMStoreFloat4x4(&worldInverseT, DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(matrix)));
	edited = 0;
}

void Transform::CalculateOrientation()
{
	relRight = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	relForward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	relUp = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation));

	DirectX::XMStoreFloat3(&relRight, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&relRight), quat));
	DirectX::XMStoreFloat3(&relUp, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&relUp), quat));
	DirectX::XMStoreFloat3(&relForward, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&relForward), quat));

	orientEdited = 0;
}