#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>


#include "Graphics.h" // To draw on screen from the object itself
#include "Vertex.h" // To define vertices

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "PathHelpers.h"

enum Kind 
{
	Box, 
	SmoothLine, 
	Circle
};

/// <summary>
/// A way to draw a 2D box for UI
/// Every box would have a constant vertex count -> 4, and index count -> 6
/// </summary>
class BorderedBox 
{

public:

	BorderedBox();
	void CreateGeometry();
	void LoadShaders();
	void Draw(float deltaTime);

	void SetWidthHeight(float x, float y);
	void SetXY(float x, float y);
	void SetColor(float r, float g, float b);
	void SetInvSmoothIntensity(float i);
	void SetOpacity(float opacity);
	void SetKind(Kind k = Box);
	

private:

	int vertexCount, indexCount, smoothVertCount, smoothIndexCount;
	float angle;
	
	BoxData data;

	// We use ComPtr because they are smart pointers, that is, they clean up after themselves. 
	
	// -- D3D11_ vs ID3D11 --

	// ID3D11 variables are ALWAYS pointers. 
	// An ID3D11* (pointer) holds several bits of data in system RAM. 
	// In the case of the ID3D11Buffer pointer, one of the bits of information IS the raw address of the buffer on the GPU. 
	// However, we cannot access this raw memory address. 
	// Even during runtime, the GPU might reorganise resources, meaning the raw address changes!

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> smoothLineVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> smoothLineIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> circlePixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> smoothLinePixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;

	ID3DBlob* vertexShaderBlob;
	ID3DBlob* pixelShaderBlob;

	std::wstring vertexShaderPath;
	std::wstring pixelShaderPath;
	std::wstring smoothLinePSPath;
	std::wstring circlePSPath;

	Kind kind;


};