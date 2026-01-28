#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class Entity 
{
public:
	Entity();
	
	
	void SetVS(Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader);
	void SetPS(Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPS;

	void Draw();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};
