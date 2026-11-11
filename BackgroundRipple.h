#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "Input.h"
#include "Vertex.h" // To define vertices
#include "Window.h" // To fetch screen size
#include "Graphics.h" // For everything -> resource creation, binding and drawing

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "PathHelpers.h"


class BackgroundRipple
{
public:
	BackgroundRipple(float width, float height);
	void CreateGeometry();
	void LoadShaders();
	void LoadTextures();
	void LoadSampler();
	void Update(); // check for mouse press here
	void UpdateBuffers(); // run ripple algorithm here
	void Draw(float deltaTime);

private:
	float width, height;
	int frameX, frameY;
	float* alphaVals;
	bool mousePressed;
	float timePassed;

	float* previousData;
	float* currentData;
	bool isCurrent;
	RippleData constData;
	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> previous; 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> current;
	
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> previousSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> currentSRV;
	
	ID3D11UnorderedAccessView* v1[3];

	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> previousUAV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> currentUAV;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	ID3DBlob* vertexShaderBlob;
	ID3DBlob* pixelShaderBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> displayPixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> calcPixelShader;
	

	std::wstring vertexShaderPath;
	std::wstring displayPixelShaderPath;
	std::wstring calcPixelShaderPath;

	int vertexCount, indexCount;
};

