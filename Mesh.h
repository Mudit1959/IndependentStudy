// Meshes need to hold all vertex data, index order data. 
// Meshes need to create vertex buffer, index buffer, and input layout
// Meshes need to have the correct topology and input layout used before each render
// Meshes need to bind active vertex buffer, index buffer, before drawing. 

#pragma once

#include "Vertex.h"
#include "Graphics.h"

#include <DirectXMath.h>

// Load vertex data
#include <fstream>
#include <vector>
#include <stdexcept>

class Mesh 
{
public:
	Mesh(Vertex* vertices,  int vCount, unsigned int* indices,  int iCount);
	Mesh(const char* objFilePath);
	void Draw();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	void CreateVertexBuffer(Vertex* vertices,  int v);
	void CreateIndexBuffer(unsigned int* indices,  int i);
	
	int indexCount, vertexCount;
};
