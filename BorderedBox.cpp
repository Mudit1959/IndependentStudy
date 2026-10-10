#include "BorderedBox.h"
#include "Window.h"

using namespace DirectX;

const float blendFactor[4] = {0.0f,0.0f, 0.0f, 0.0f};

BorderedBox::BorderedBox() 
{
	vertexCount = 4;
	indexCount = 6;

	pixelShaderPath = L"PixelShader.cso"; // Path to Pixel Shader
	vertexShaderPath = L"VertexShader.cso"; // Path to Vertex Shader
}

void BorderedBox::CreateGeometry()
{

	// -- COLORS -- 
	// defining colors for ease of use. 
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// -- VERTEX ARRAY --
	// Holds all the vertices necessary to display a quad. 
	// The vertex coordinates are known as "normalized device coordinates" or "homogenous screen coordinates". 
	// These coordinates represent a position in space on the screen, relative to the screen resolution
	// Rescaling the screen would rescale the quad being rendered too. 
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), red },
		{ XMFLOAT3(+1.0f, +0.0f, +0.0f), blue },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), green },
		{ XMFLOAT3(0.0f, -1.0f, +0.0f), blue}
	};

	// -- INDICES ARRAY --
	// The order in which to render vertices on-screen (clockwise)
	// Every quad is made up of 2 triangles
	// So we feed in 6 indices -> 3 each to define the triangles. 
	// An indices array saves memory when rendering millions of quads. 
	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	// -- CONSTANTS --
	data.screenSize.x = Window::Width();
	data.screenSize.y = Window::Height();
	data.boxSize = DirectX::XMFLOAT2(200, 200);
	data.offset = DirectX::XMFLOAT2(100.0f, 50.0f);
	data.opacity = 0.2;
	
	// -- GRAPHICS -- 
	{// 
	// Graphics Device -> Used to CREATE AND DEFINE resources and API state objects. 
	// Resources are sets of rendering data stored on the GPU for drawing each object. 
	// API State objects define the manner objects should be drawn (rendering options). 
	// 
	// Graphics Device Context -> Used to SET AND DECOUPLE resources, and API state objects. Issues all rendering commands.
	// 
	// Swap Chain -> Function for presenting the final pixels on screen.
	}

	// -- RESOURCE CREATION --
	// 1) Define the kind of memory buffer you need.
	// 2) Optionally initialize it with some pre-defined data.
	// 3) Ask the API to create it. 

	// .Get vs .GetAddressOf
	{
		// For ComPtrs, 
		// .Get() returns the raw underlying C++ pointer
		// However, for storing the object created by the D3D API, we need the address of the C++ pointer!
		// .GetAddressOf() returns the address of the C++ pointer. 
	}

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

	// -- CONSTANT BUFFER --
	{
		D3D11_BUFFER_DESC cbd = {};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = ((sizeof(BoxData) + 15) / 16) * 16;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Graphics::Device->CreateBuffer(&cbd, 0, constantBuffer.GetAddressOf());
	}
}

void BorderedBox::LoadShaders() 
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

	// -- INPUT LAYOUT --
	{
		// Input Layouts describe the way vertices should be fed into the Vertex Shader. 
		// Crucial to get right. 
		// The rest of the pieline breaks if the input layout does not match the vertex struct in the vertex shader.
		//

		// For each data member in a vertex, create a description
		D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};

		
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// What kind of data member is it? (3 floats, one int, 2 doubles, etc.)	
		inputElements[0].SemanticName = "POSITION";				// What semantic is used to refer to it within the Vertex shader?
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Is there any offset? Or does the data member come right after the previous one?

		
		inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			
		inputElements[1].SemanticName = "COLOR";							
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	

		// Create the input layout, referring to the descriptions made, and the vertex shader blob.
		Graphics::Device->CreateInputLayout(
			inputElements,							// Array of descriptions
			2,										// How many data members does each vertex have?
			vertexShaderBlob->GetBufferPointer(),	// Where is the vertex shader blob located?
			vertexShaderBlob->GetBufferSize(),		// How big is the vertex shader blob?
			inputLayout.GetAddressOf());			// Where should the D3D11 Input Layout be linked once created?
	}

	// -- BLEND STATE -- 
	{
		D3D11_BLEND_DESC blendStateDesc = {};
		blendStateDesc.AlphaToCoverageEnable = false;
		blendStateDesc.IndependentBlendEnable = false;
		blendStateDesc.RenderTarget[0].BlendEnable = true;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Graphics::Device->CreateBlendState(&blendStateDesc, blendState.GetAddressOf());
	}
}

// -- RENDERING PIPELINE --

// -- INPUT ASSEMBLER -- 
// The vertex and index buffers have been created on the GPU. The state of the GPU has been described. 
// Based on the topology set, the vertices are grouped into primitives (shapes). 
// The vertices are the "input". The input assembler launches N instances of the vertex shader, where N = total number of vertices in the vertex buffer
// 
// -- VERTEX SHADER -- 
// Shaders are scripts that can be run in parallel on the GPU. Each shader serves a different purpose. 
// Shaders must be provided. If nothing is provided, then the pipeline does not work
// Shaders can be fed in bits of constant data through a constant buffer.
// Vertex shaders are used to transform and project each vertex through matrix multiplication. 
// Transforming and projecting is necessary to depict 3D objects accurately. They convert the vertex coordinates from their local space to global space.
// The vertex shader also passes on the data unique to each vertex -> color, UVs, normals, etc.
// 
// -- RASTERIZER --
// Rasterize -> To determine the pixels on screen that make up a projected primitive(triangle). 
// Rasterizers improve runtime performance by determining what should not be rendered -> (back facing the camera, offscreen clipping)
// Rasterizers also interpolate the data across pixels from vertex to vertex, crucial for implementing object shading in the pixel shader. 
// Rasterizers launch N instances of the pixel shader, where N = total number of rasterized pixels, that is, pixels making up primitives!
// 
// -- PIXEL SHADER --
// Must return, at the very least an RGBA value
// For every RASTERIZED pixel, not for every pixel on screen. 
// This is the costliest part of the pipeline. All the texturing, lighting effects are applied here. 
// 
// -- OUTPUT MERGER --
// The Ouput Merger takes the results of the pixel shader for every object in scene and "merges" them. 
// For 3D environments, the depth buffer must be enabled. The depth buffer tracks the distance of a surface from the "camera" via a pixel color. 
// The pixel color in the depth buffer is only changed if it detects an object closer to the camera?
// By default, the Ouput Merger renders objects on top of each other, with NO REGARD FOR ALPHA VALUES. 
// D3D11 follows a "double buffering" setup. There are two ouput buffers (2D array of pixel color values) used to display results on-screen.
// The back buffer is written to, and stores the rendering results as they are calculated. 
// The front buffer is the frame being presented to the person. 
// The front and back buffers are swapped every frame. 
// 


void BorderedBox::Draw()
{
	{
		UINT stride = sizeof(Vertex); // How big is one vertex of this particular object?
		UINT offset = 0; // Start from the beginning or some bytes after?
		data.screenSize.x = Window::Width();
		data.screenSize.y = Window::Height();

		// -- BRIEF --
		{
			// Before we draw, we need to know which vertices to render, in what order, in what manner, which shaders to use, at minimum.
			// 
			// Several other options such as which faces to draw/cull, whether to fill in objects or render them as wireframes,
			// whether to use a depth buffer or not, must also be answered 
			// 
			// An API state object known as a Rasterizer State Object, holds some of these settings
			// An API state object is a resource too! Necessary for rendering data. Thus, it follows the same flow as RESOURCE CREATION
			// Minimum non-zero values for a Rasterizer State Description needed are: cull mode(what to ignore), fill mode(wireframe, solid, etc.), depth buffer (depth clip enable)
			//  
		}

		// -- SET TOPOLOGY --
		{
			// The kind of shapes we are drawing -> lines, triangles, interjoined strips, etc.
			Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		// -- SET INPUT LAYOUT -- 
		{
			Graphics::Context->IASetInputLayout(inputLayout.Get());
		}

		// -- SET SHADERS -- 
		{
			Graphics::Context->VSSetShader(vertexShader.Get(), 0, 0);
			Graphics::Context->PSSetShader(pixelShader.Get(), 0, 0);
		}

		// -- BIND VERTEX AND INDEX BUFFERS --
		{
			Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
			Graphics::Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}

		// -- WRITE TO AND BIND CONSTANT BUFFER --
		{
			// Writing data to buffer
			D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
			Graphics::Context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
			memcpy(mappedBuffer.pData, &data, sizeof(BoxData));
			Graphics::Context->Unmap(constantBuffer.Get(), 0);

			// Binding buffer
			Graphics::Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		}

		// -- SET OUTPUT MERGER STATE / BLEND STATE --
		{
			
			Graphics::Context->OMSetBlendState(blendState.Get(),blendFactor,0xffffff);
		}
		
		Graphics::Context->DrawIndexed(
			6,
			0,
			0);
	}
}