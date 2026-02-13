#include "RectangleShape.h"


RectangleShape::RectangleShape(std::shared_ptr<Material> inMaterial)
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	material = inMaterial;
}

void RectangleShape::CreateVertexBuffer()
{
	// Create a VERTEX BUFFER
	// - This holds the vertex data of triangles for a single object
	// - This buffer is created on the GPU, which is where the data needs to
	//    be if we want the GPU to act on it (as in: draw it to the screen)

	// First, we need to describe the buffer we want Direct3D to make on the GPU
	//  - Note that this variable is created on the stack since we only need it once
	//  - After the buffer is created, this description variable is unnecessary
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;       // Depends on number of vertices
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells Direct3D this is a vertex buffer
	vbd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we initially fill the buffer with data
	// - Essentially, we're specifying a pointer to the data to copy
	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	initialVertexData.pSysMem = vertices; // pSysMem = Pointer to System Memory

	// Actually create the buffer on the GPU with the initial data
	// - Once we do this, we'll NEVER CHANGE DATA IN THE BUFFER AGAIN
	Graphics::Device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());
}

void RectangleShape::CreateIndexBuffer()
{
	// Creates an INDEX Buffer

	D3D11_BUFFER_DESC ibDesc = {}; // Index buffers need their own buffer description, contained in a struct
	ibDesc.Usage = D3D11_USAGE_IMMUTABLE; // Will data be editable in this index buffer
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // What should this chunk of data be treated as -> in this case, INDEX BUFFER
	ibDesc.ByteWidth = sizeof(unsigned int) * indexCount; // How big is the chunk of data
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0; // Any offset from the beginning of the data?

	D3D11_SUBRESOURCE_DATA initialIndexData = {}; // Tell the GPU to create space 
	initialIndexData.pSysMem = indices; // Tell the GPU where to fetch data from

	Graphics::Device->CreateBuffer(&ibDesc, &initialIndexData, indexBuffer.GetAddressOf()); // Create the buffer
}

void RectangleShape::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	Graphics::Context->IASetInputLayout(material->GetInputLayout().Get());
	Graphics::Context->VSSetShader(material->GetVertexShader().Get(), 0, 0);
	Graphics::Context->PSSetShader(material->GetPixelShader().Get(), 0, 0);

	Graphics::Context->DrawIndexed(indexCount, 0, 0);
}

Transform* RectangleShape::GetTransform() { return &transform; }