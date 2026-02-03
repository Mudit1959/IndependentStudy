#pragma once
#include "DirectXMath.h"

class Transform
{
public:
	Transform();

	void SetPosition(DirectX::XMFLOAT3 inPos);
	void SetPosition(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 inScale);
	void SetScale(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 inRot);
	void SetRotation(float x, float y, float z);


	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldInverseTMatrix();
	
	void MoveAbsolute(DirectX::XMFLOAT3 moveBy);
	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(DirectX::XMFLOAT3 moveBy);
	void MoveRelative(float x, float y, float z);

	void Rotate(DirectX::XMFLOAT3 rotateBy);
	void Rotate(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scaleBy);
	void Scale(float x, float y, float z);

	void CalculateWorldMatrix();
	void CalculateOrientation();

private:
	DirectX::XMFLOAT3 scale, rotation, position; 
	DirectX::XMFLOAT3 relUp, relRight, relForward;
	DirectX::XMFLOAT4X4 world, worldInverseT;
	int edited;
};

