#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "BufferStructs.h"

// ImGui header files
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"




#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

std::vector<Entity> entityList;

std::vector<RectangleShape> rectList;
std::vector<CircleShape> circleList;


float rectPos[2] = { 10.0f, 10.0f};
float cameraPos[3] = { 0.0f, 0.0f, 0.0f };
float scale[3] = { 10.0f, 10.0f, 1.0f };
float circleScale[2] = {10.0f, 10.0f};
unsigned int screenWidth, screenHeight;

DirectX::XMFLOAT4 hoverColor(1.0f, 0.0f, 0.0f, 1.0f);

// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	screenWidth = Window::Width();
	screenHeight = Window::Height();

	LoadShadersCreateMaterials();
	CreateGeometry();
	

	Game::ImGuiInitialize();


	CreateCamera();

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	
}

void Game::ImGuiInitialize() 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Window::Handle());
	ImGui_ImplDX11_Init(Graphics::Device.Get(), Graphics::Context.Get());
	ImGui::StyleColorsDark();
}

void Game::ImGuiUpdate(float deltaTime) 
{
	// Feed fresh data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)Window::Width();
	io.DisplaySize.y = (float)Window::Height();
	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Relay new input from mouse and keyboard
	Input::SetKeyboardCapture(io.WantCaptureKeyboard);
	Input::SetMouseCapture(io.WantCaptureMouse);

	Game::ImGuiBuildUI();
}

void Game::ImGuiBuildUI() 
{
	ImGui::Begin("Hello There!");
	if (ImGui::TreeNode("Rectangle World Pos")) 
	{
		ImGui::DragFloat2("", &rectPos[0], 0.1f, 0.0f, 720.0f, "%.3f");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Rectangle Half Dimensions"))
	{
		ImGui::DragFloat("Half-Width", &scale[0], 0.1f, 0.5f, 500.0f, "%.3f");
		ImGui::DragFloat("Half-Height", &scale[1], 0.1f, 0.5f, 500.0f, "%.3f");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Circle Scale"))
	{
		ImGui::DragFloat2("", &circleScale[0], 0.1f, 0.0f, 360.0f, "%.3f");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Camera Pos"))
	{
		ImGui::DragFloat3("", &cameraPos[0], 0.01f, -50.0f, 50.0f, "%.3f");
		ImGui::TreePop();
	}
	ImGui::End();
}

// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// Clean Up ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Game::LoadVertexShader(const std::wstring& path) 
{
	vertexShaders.push_back(Microsoft::WRL::ComPtr<ID3D11VertexShader>());
	
	ID3DBlob* vertexShaderBlob;

	// BLOb -> Binary Large Object files! Basically huge chunks of data of the compiled shader bytecode
	// We basically plop this data into a buffer to read from.
	// This is to create the vertex shader and associated input layout properly.
	// Using Chris Cascioli's FixPath(), to ensure correct relative file path navigation - Note 'L' for wide strings!

	HRESULT check = D3DReadFileToBlob(FixPath(path).c_str(), &vertexShaderBlob);

	Graphics::Device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),	// Point to the blob's data - stored in a "buffer"
		vertexShaderBlob->GetBufferSize(),		// How big is that data?
		0,										// Any classes? - No classes in this shader
		vertexShaders[vertexShaders.size() - 1].GetAddressOf());			// The address of the ID3D11VertexShader pointer


	if (path == L"VertexShader.cso") { Game::DefaultInputLayout(vertexShaderBlob); }
}


void Game::LoadPixelShader(const std::wstring& path) 
{
	pixelShaders.push_back(Microsoft::WRL::ComPtr<ID3D11PixelShader>());

	ID3DBlob* pixelShaderBlob;

	D3DReadFileToBlob(FixPath(path).c_str(), &pixelShaderBlob);

	Graphics::Device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),	// Point to blob's data - stored in a "buffer"
		pixelShaderBlob->GetBufferSize(),		// How big is the data?
		0,										// Any classes? - None in this shader
		pixelShaders[pixelShaders.size() - 1].GetAddressOf()	// Address of the ID3D11PixelShader pointer
	);
}

// --- INPUT LAYOUT METHODS ---
void Game::DefaultInputLayout(ID3DBlob* vertexShaderBlob)
{
	inputLayouts.push_back(Microsoft::WRL::ComPtr<ID3D11InputLayout>());

	D3D11_INPUT_ELEMENT_DESC inputLayout[2] = {};

	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	inputLayout[1].SemanticName = "COLOR";
	inputLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	Graphics::Device->CreateInputLayout(
		inputLayout,
		2,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		inputLayouts[inputLayouts.size() - 1].GetAddressOf());

}


// --------------------------------------------------------
// Loads the shaders using their precompiled bytecode files (.cso)
// 
// Whenever a vertex shader is loaded, it is checked against a list of input layouts
// If the required input layout has not been created on the GPU, it will be created, provided the LoadVertexShader function is coded correctly!
// Up to me to keep track of different input layout requirements
// --------------------------------------------------------
void Game::LoadShadersCreateMaterials()
{
	
	Game::LoadVertexShader(L"VertexShader.cso");
	Game::LoadPixelShader(L"PixelShader.cso");
	

	// At this stage -> 
	// Vertex Shaders - 1 
	// Pixel Shaders - 1
	// Input Layouts - 1

	materials.push_back(std::make_shared<Material>(vertexShaders[0], pixelShaders[0], inputLayouts[0]));

	Game::LoadVertexShader(L"ButtonVS.cso");


	materials.push_back(std::make_shared<Material>(vertexShaders[1], pixelShaders[0], inputLayouts[0]));
	
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	CreateEntities();
}

void Game::CreateEntities()
{
	rectList.push_back(RectangleShape(materials[1]));
	circleList.push_back(CircleShape(materials[1]));
}

void Game::CreateCamera()
{
	camera = std::make_shared<Camera>(Window::AspectRatio());
	cameraPos[2] -= 10.0f;
	camera->GetTransform()->SetPosition(DirectX::XMFLOAT3(&cameraPos[0]));
}

// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	camera->Resize(Window::AspectRatio());
	screenWidth = Window::Width();
	screenHeight = Window::Height();
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	Game::ImGuiUpdate(deltaTime);
	
	rectList[0].GetTransform()->SetScale(DirectX::XMFLOAT3(&scale[0]));
	rectList[0].GetTransform()->Rotate(0.0f, 0.0f, deltaTime * DirectX::XM_PI / 18.0f);
	rectList[0].GetTransform()->CalculateWorldMatrix();

	circleList[0].GetTransform()->SetScale(circleScale[0], circleScale[1], 1.0f);
	circleList[0].GetTransform()->CalculateWorldMatrix();

	camera->GetTransform()->SetPosition(DirectX::XMFLOAT3(&cameraPos[0]));

	camera->Update();

	Game::CheckMousePosition();

	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();
}

void Game::CheckMousePosition()
{
	int mouseX = Input::GetMouseX();
	int mouseY = Input::GetMouseY();

	if (mouseX < rectPos[0] - scale[0] || mouseX > rectPos[0] + scale[0] || mouseY < rectPos[1] || mouseY - scale[0] > rectPos[1] + scale[1]) { DirectX::XMStoreFloat4(&hoverColor, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f)); }
	else { DirectX::XMStoreFloat4(&hoverColor, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f)); }
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erase what's on screen) and depth buffer
		const float backBufferClear[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	backBufferClear);
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	

	// DRAW geometry
	// - These steps are generally repeated for EACH object you draw
	// - Other Direct3D calls will also be necessary to do more complex things
	{
		for (int i = 0; i < rectList.size(); i++) 
		{
			SetConstantsForFrame(rectList[i], 1);
			rectList[i].Draw();
		}

		for (int i = 0; i < circleList.size(); i++)
		{
			SetConstantsForFrame(circleList[i], 1);
			circleList[i].Draw();
		}
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Draw ImGui
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Present at the end of the frame
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Re-bind back buffer and depth buffer after presenting
		Graphics::Context->OMSetRenderTargets(
			1,
			Graphics::BackBufferRTV.GetAddressOf(),
			Graphics::DepthBufferDSV.Get());
	}
}

void Game::SetConstantsForFrame(RectangleShape e, int kind) 
{
	if(kind == 0)
	{
		VertexShaderConstants vsData = {};
		vsData.world = e.GetTransform()->GetWorldMatrix();
		vsData.worldInvT = e.GetTransform()->GetWorldInverseTMatrix();

		//DirectX::FXMMATRIX cameraView = DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		//DirectX::XMStoreFloat4x4(&vsData.view, cameraView);
		vsData.view = camera->GetViewMatrix();

		//DirectX::FXMMATRIX cameraProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, Window::AspectRatio(), 0.01f, 400.0f);
		//DirectX::XMStoreFloat4x4(&vsData.proj, cameraProj);
		vsData.proj = camera->GetProjMatrix();

		Graphics::FillAndBindNextConstantBuffer(&vsData, sizeof(VertexShaderConstants), D3D11_VERTEX_SHADER, 0);
	}

	if (kind == 1) 
	{
		ButtonShaderConstants buttonVSData = {};
		buttonVSData.world = e.GetTransform()->GetWorldMatrix();
		buttonVSData.worldInvT = e.GetTransform()->GetWorldInverseTMatrix();
		buttonVSData.screenWH = DirectX::XMINT2(screenWidth, screenHeight);
		buttonVSData.translateXY = DirectX::XMFLOAT2(&rectPos[0]);
		buttonVSData.colour = hoverColor;

		Graphics::FillAndBindNextConstantBuffer(&buttonVSData, sizeof(ButtonShaderConstants), D3D11_VERTEX_SHADER, 0);
	}
	
}

void Game::SetConstantsForFrame(CircleShape e, int kind)
{
	if (kind == 0)
	{
		VertexShaderConstants vsData = {};
		vsData.world = e.GetTransform()->GetWorldMatrix();
		vsData.worldInvT = e.GetTransform()->GetWorldInverseTMatrix();

		//DirectX::FXMMATRIX cameraView = DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		//DirectX::XMStoreFloat4x4(&vsData.view, cameraView);
		vsData.view = camera->GetViewMatrix();

		//DirectX::FXMMATRIX cameraProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, Window::AspectRatio(), 0.01f, 400.0f);
		//DirectX::XMStoreFloat4x4(&vsData.proj, cameraProj);
		vsData.proj = camera->GetProjMatrix();

		Graphics::FillAndBindNextConstantBuffer(&vsData, sizeof(VertexShaderConstants), D3D11_VERTEX_SHADER, 0);
	}

	if (kind == 1)
	{
		ButtonShaderConstants buttonVSData = {};
		buttonVSData.world = e.GetTransform()->GetWorldMatrix();
		buttonVSData.worldInvT = e.GetTransform()->GetWorldInverseTMatrix();
		buttonVSData.screenWH = DirectX::XMINT2(screenWidth, screenHeight);
		buttonVSData.translateXY = DirectX::XMFLOAT2(360, 360);
		buttonVSData.colour = White;

		Graphics::FillAndBindNextConstantBuffer(&buttonVSData, sizeof(ButtonShaderConstants), D3D11_VERTEX_SHADER, 0);
	}

}

