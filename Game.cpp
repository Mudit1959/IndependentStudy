#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"

#include "BorderedBox.h"
#include "Branch.h"

// ImGui header files
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"


#include <DirectXMath.h>



// For the DirectX Math library
using namespace DirectX;


Branch* branch;
BorderedBox* renderBox;

float whxy[4];
float objColor[4] = {255, 0, 0, 100};


// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	Game::ImGuiInitialize();
	whxy[0] = 50;
	whxy[1] = 50;
	whxy[2] = 100;
	whxy[3] = 50;
	renderBox = new BorderedBox();
	renderBox->CreateGeometry();
	renderBox->LoadShaders();

	branch = new Branch(80, 80, 100, 50, 50);
	branch->AddNode(40, -20);
	
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
	ImGui::Begin("Testing...");
	
	if (ImGui::TreeNode("Box Constants")) 
	{
		static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
		ImGui::DragFloat("DragFloat (0 -> Screen Width)", &whxy[2], 0.5f, 0.0f, Window::Width(), "%.3f", flags);
		ImGui::DragFloat("DragFloat (0 -> Screen Height)", &whxy[3], 0.5f, 0.0f, Window::Height(), "%.3f", flags);
		ImGui::InputFloat("Width", &whxy[0], 0, 0);
		ImGui::InputFloat("Height", &whxy[1], 0, 0);
		ImGui::ColorEdit4("Color", &objColor[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_None);
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

	// Clean Up BorderedBox object
	delete branch;
	delete renderBox;
	
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	Game::ImGuiUpdate(deltaTime);

	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();
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
		// 0.4f, 0.6f, 0.75f, 0.0f
		const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	color);
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// Frame DRAW
	{
		branch->Draw(deltaTime, renderBox);
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



