#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include <d3d11shadertracing.h> // Required to use D3D11_SHADER_TYPE
#include <string>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace Graphics
{
	// --- GLOBAL VARS ---

	// Primary D3D11 API objects
	inline Microsoft::WRL::ComPtr<ID3D11Device1> Device;
	inline Microsoft::WRL::ComPtr<ID3D11DeviceContext1> Context;
	inline Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;

	// Constant Ring Buffer
	inline Microsoft::WRL::ComPtr<ID3D11Buffer> ConstBufferHeap; // Pointer to D3D11 Buffer object
	inline unsigned int cbSizeBytes; // Size of buffer in bytes
	inline unsigned int cbOffsetBytes;

	// Rendering buffers
	inline Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	inline Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthBufferDSV;

	// Debug Layer
	inline Microsoft::WRL::ComPtr<ID3D11InfoQueue> InfoQueue;

	// --- FUNCTIONS ---

	// Getters
	bool VsyncState();
	std::wstring APIName();

	// General functions
	HRESULT Initialize(unsigned int windowWidth, unsigned int windowHeight, HWND windowHandle, bool vsyncIfPossible);
	void ShutDown();
	void ResizeBuffers(unsigned int width, unsigned int height);

	// Constant Ring Buffer
	void FillAndBindNextConstantBuffer(void* data, unsigned int dataSize, D3D11_SHADER_TYPE type, unsigned int registerSlot);

	// Debug Layer
	void PrintDebugMessages();
}