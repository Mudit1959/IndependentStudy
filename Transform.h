#pragma once
#include "DirectXMath.h"

// Using DirectX storage formats (XMFLOAT3, XMFLOAT4X4)
// Used to calculate world matrix of objects, and view matrix of camera!

class Transform 
{
public:
	Transform();
	
	// Setters
	void SetPosition(DirectX::XMFLOAT3 inPos);
	void SetPosition(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 inScale);
	void SetScale(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 inRot);
	void SetRotation(float x, float y, float z);

	// Movements
	void MoveAbsolute(DirectX::XMFLOAT3 move);
	void MoveAbsolute(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scale);
	void Scale(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 rotate);
	void Rotate(float x, float y, float z);

	// Calculate
	void CalculateWorldMatrix();

	// Getters
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetPitchYawRoll();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetRight();
	DirectX::XMFLOAT3 GetUp();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldInverseMatrix();

private:
	DirectX::XMFLOAT3 position, scale, rotation;
	DirectX::XMFLOAT3 relUp, relForward, relRight;
	DirectX::XMFLOAT4X4 world, worldInvT;

	int edited; // to check for recalculation before submitting a matrix!
};