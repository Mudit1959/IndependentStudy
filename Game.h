#pragma once

#include <d3d11.h>
#include <wrl/client.h>



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

private:
};

