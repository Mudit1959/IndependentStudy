#include "Material.h"

Material::Material(Microsoft::WRL::ComPtr<ID3D11VertexShader> vs, Microsoft::WRL::ComPtr<ID3D11PixelShader> ps, Microsoft::WRL::ComPtr<ID3D11InputLayout> ia) 
{
	vertexShader = vs;
	pixelShader = ps;
	inputLayout = ia;
}


Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader() { return vertexShader; }
Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader() { return pixelShader; }
Microsoft::WRL::ComPtr<ID3D11InputLayout> Material::GetInputLayout() { return inputLayout; }