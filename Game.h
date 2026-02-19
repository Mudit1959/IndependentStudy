#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

// Classes 
#include "Mesh.h"
#include "Material.h"
#include "Entity.h"
#include "RectangleEntity.h"
#include "Camera.h"

class Game
{
public:
	// Basic OOP setup
	Game();
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

	// ImGui
	void ImGuiInitialize();
	void ImGuiUpdate(float deltaTime);
	void ImGuiBuildUI();

	//Mouse
	void CheckMousePosition();

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShadersCreateMaterials();
	void CreateGeometry();
	void CreateCamera();
	void LoadVertexShader(const std::wstring& path);
	void LoadPixelShader(const std::wstring& path);
	void DefaultInputLayout(ID3DBlob* vertexShaderBlob);

	void SetConstantsForFrame(RectangleEntity e);
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Meshes
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Material>> materials;

	// Shaders - input layouts, vertex shaders, pixel shaders - stored in lists, one for each type
	std::vector<Microsoft::WRL::ComPtr<ID3D11InputLayout>> inputLayouts; // Different input layouts based on topology and meshes' vertex data structure
	std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>> vertexShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaders;

	// Shaders and shader-related constructs
	
};

