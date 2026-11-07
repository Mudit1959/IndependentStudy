#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "Input.h"
#include "Vertex.h" // To define vertices
#include "Graphics.h" // For everything -> resource creation, binding and drawing

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "PathHelpers.h"


class BackgroundRipple
{
public:
	BackgroundRipple(int width, int height);
	void CreateGeometry();
	void LoadShaders();
	void LoadTextures();
	void Update(); // check for mouse press here
	void Draw();

private:
	int screenWidth, screenHeight;
	float mouseX, mouseY;
	bool mousePressed;

	UINT* previousData;
	UINT* currentData;
	
	D3D11_TEXTURE2D_DESC textureDesc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> previous; 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> current;
	

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> previousRTV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> currentRTV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> previousSRV; 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> currentSRV;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	ID3DBlob* vertexShaderBlob;
	ID3DBlob* pixelShaderBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	

	std::wstring vertexShaderPath;
	std::wstring pixelShaderPath;

	int vertexCount, indexCount;
};

