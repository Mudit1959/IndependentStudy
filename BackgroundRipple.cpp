#include "BackgroundRipple.h"

using namespace DirectX;

BackgroundRipple::BackgroundRipple(int width, int height) 
{
	screenWidth = width;
	screenHeight = height;

	previousData = new UINT[width * height * 4];
	currentData = new UINT[width * height * 4];

	pixelShaderPath = L"ripplePixelShader.cso"; // Path to Pixel Shader
	vertexShaderPath = L"VertexShaderRipple.cso"; // Path to Vertex Shader

	CreateGeometry();
	LoadShaders();
	LoadTextures();

}

void BackgroundRipple::CreateGeometry() 
{

	// -- VERTEX AND INDEX COUNT --
	vertexCount = 4;
	indexCount = 6;

	// -- COLORS -- 
	// The ripples are opaque, but there is no solid background
	XMFLOAT4 alphaZero = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	// -- VERTEX ARRAY --
	
	// Holds all the vertices necessary to display a quad. 
	// The vertex coordinates are known as "normalized device coordinates" or "homogenous screen coordinates". 
	// These coordinates represent a position in space on the screen, relative to the screen resolution
	// Rescaling the screen would rescale the quad being rendered too. 
	
	// In this case, I want my ripple area to take up the entire screen.
	// My four vertices need to mark the 4 corners of the entire window. 

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, +0.0f), XMFLOAT2(0, 0)},
		{ XMFLOAT3(+1.0f, 1.0f, +0.0f), XMFLOAT2(1, 0) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT2(1, 1) },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT2(0, 1)}
	};

	// -- INDICES ARRAY --
	// The order in which to render vertices on-screen (clockwise)
	// Every quad is made up of 2 triangles
	// So we feed in 6 indices -> 3 each to define the triangles. 
	// An indices array saves memory when rendering millions of quads. 
	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	// -- VERTEX BUFFER --
	{
		// 1) Defining the vertex buffer characteristics
		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_IMMUTABLE; // We will never try to change data during runtime
		vbd.ByteWidth = sizeof(Vertex) * vertexCount;  // How big is the buffer?
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // What kind of buffer should it be treated as at runtime? (when binded!)
		vbd.CPUAccessFlags = 0;	// Can the CPU access this data during run-time (NO! = 0)
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		// 2) Providing pre-set vertex data
		D3D11_SUBRESOURCE_DATA initialVertexData = {}; // via a struct that points to the data on system memory 
		initialVertexData.pSysMem = vertices; // pSysMem -> Pointer to system memory

		// 3) Create the buffer

		//  For debugging, "almost all methods return an HRESULT"
		// Check if the HRESULT was S_OK, if not you can print the debug logs/error message
		//

		Graphics::Device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());
	}

	// -- INDEX BUFFER --
	{
		// 1) Defining the need for an index buffer 
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;	// Will not change data during runtime
		ibd.ByteWidth = sizeof(unsigned int) * indexCount;	// Size of the index array (how many indices? * size of each index)
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// What kind of buffer should it be treated as at runtime? (when binded!)
		ibd.CPUAccessFlags = 0;	// CPU should not be able to access the data (0)
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0; // Should I skip some initial block of data?

		// 2) Optionally loading it with pre-defined indices order
		D3D11_SUBRESOURCE_DATA initialIndexData = {};
		initialIndexData.pSysMem = indices; // pSysMem -> pointer to System Memory

		// 3) Asking the GPU to create this resource via the API. 
		Graphics::Device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());
	}


}

void BackgroundRipple::LoadShaders() 
{
	// -- SHADERS --
	{
		// Shaders are not typical resources. 
		// They need to be precompiled. This is done by including <d3dcompiler.h>
		// They do not require a resource description, but they do need a blob.

		// Every shader is first read into a Binary Large OBject (blob)
		// Essentially, a big chunk of data. 
		D3DReadFileToBlob(FixPath(pixelShaderPath).c_str(), &pixelShaderBlob);
		D3DReadFileToBlob(FixPath(vertexShaderPath).c_str(), &vertexShaderBlob);


		Graphics::Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(), // Where is the blob located?	
			pixelShaderBlob->GetBufferSize(),	// How big is the blob?
			0,
			pixelShader.GetAddressOf());		// Where should the D3D11 object be linked once created?

		Graphics::Device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),	// Where is the blob located?
			vertexShaderBlob->GetBufferSize(),		// How big is the blob?
			0,
			vertexShader.GetAddressOf());			// Where should the D3D11 object be linked once created?
	}

	// The vertex shader blob MUST NOT BE EDITED before creating the necessary input layout. 

	// -- INPUT LAYOUT --
	{
		// Input Layouts describe the way vertices should be fed into the Vertex Shader. 
		// Crucial to get right. 
		// The rest of the pieline breaks if the input layout does not match the vertex struct in the vertex shader.
		//
		const int inputNums = 2;
		// For each data member in a vertex, create a description
		D3D11_INPUT_ELEMENT_DESC inputElements[inputNums] = {};


		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// What kind of data member is it? (3 floats, one int, 2 doubles, etc.)	
		inputElements[0].SemanticName = "POSITION";				// What semantic is used to refer to it within the Vertex shader?
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Is there any offset? Or does the data member come right after the previous one?

		inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElements[1].SemanticName = "UVCOORD";
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		// Create the input layout, referring to the descriptions made, and the vertex shader blob.
		Graphics::Device->CreateInputLayout(
			inputElements,							// Array of descriptions
			inputNums,										// How many data members does each vertex have?
			vertexShaderBlob->GetBufferPointer(),	// Where is the vertex shader blob located?
			vertexShaderBlob->GetBufferSize(),		// How big is the vertex shader blob?
			inputLayout.GetAddressOf());			// Where should the D3D11 Input Layout be linked once created?
	}

	// -- BLEND STATE -- 
	{
		// REMEMBER - For this blend state to take proper effect, the Z BUFFER MUST BE DISABLED!!!
		// This is done through the Graphics helper functions EnableZBuffer and DisableZBuffer

		D3D11_BLEND_DESC blendStateDesc = {};
		blendStateDesc.IndependentBlendEnable = false;
		blendStateDesc.AlphaToCoverageEnable = false;

		blendStateDesc.RenderTarget[0].BlendEnable = true;

		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Graphics::Device->CreateBlendState(&blendStateDesc, blendState.GetAddressOf());
	}
}

void BackgroundRipple::LoadTextures() 
{
	// The pixel shader will be sampling 2 textures. 
	// The 2 textures hold the values of the last two frames. 
	// Frame 1 -> Stored in Previous -> Ripple create
}