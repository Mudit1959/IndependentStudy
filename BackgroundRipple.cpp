#include "BackgroundRipple.h"

using namespace DirectX;

const int channelSize = 1;
unsigned long long dataTypeSize = sizeof(float);


BackgroundRipple::BackgroundRipple(float w, float h)
{
	
	width = w;
	height = h;
	printf("%d",dataTypeSize);

	

	constData.backgroundSize = DirectX::XMFLOAT2(w, h);

	displayPixelShaderPath = L"rippleDisplayPixelShader.cso"; // Path to Pixel Shader for display
	calcPixelShaderPath = L"rippleCalcPixelShader.cso"; // Path to Pixel Shader for calculation
	vertexShaderPath = L"rippleVertexShader.cso"; // Path to Vertex Shader

	CreateGeometry();
	LoadShaders();
	LoadTextures();
	LoadSampler();
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
		{ XMFLOAT3(0.0f, 0.0f, +0.0f), XMFLOAT2(0, 0)},
		{ XMFLOAT3(+1.0f, 0.0f, +0.0f), XMFLOAT2(1, 0) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT2(1, 1) },
		{ XMFLOAT3(0.0f, -1.0f, +0.0f), XMFLOAT2(0, 1)}
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
		D3DReadFileToBlob(FixPath(displayPixelShaderPath).c_str(), &pixelShaderBlob);
		D3DReadFileToBlob(FixPath(vertexShaderPath).c_str(), &vertexShaderBlob);


		Graphics::Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(), // Where is the blob located?	
			pixelShaderBlob->GetBufferSize(),	// How big is the blob?
			0,
			displayPixelShader.GetAddressOf());		// Where should the D3D11 object be linked once created?

		Graphics::Device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),	// Where is the blob located?
			vertexShaderBlob->GetBufferSize(),		// How big is the blob?
			0,
			vertexShader.GetAddressOf());			// Where should the D3D11 object be linked once created?
	}

	// The vertex shader blob MUST NOT BE EDITED before creating the necessary input layout. 
	// I can reuse the pixel shader blob to create more pixel shaders
	D3DReadFileToBlob(FixPath(calcPixelShaderPath).c_str(), &pixelShaderBlob);
	Graphics::Device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(), // Where is the blob located?	
		pixelShaderBlob->GetBufferSize(),	// How big is the blob?
		0,
		calcPixelShader.GetAddressOf());		// Where should the D3D11 object be linked once created?

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
	
	// We are creating our own textures. These textures must occupy a fixed space on screen.
	// I want these textures to be of the same resolution as their space on screen before I decide to downscale/upscale. 
	// Every texture requires a description to be created. 

	// Trying a different method - store the alpha value and inverse alpha value of the ripple in the textures, and output to screen

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1; // How many times are we halving this texture to create different resolutions to sample from? -> In this case, we do not want any lower levels of sampling than just 1 texture
	td.ArraySize = 1; // Only more than 1 if we are creating a Texture2D array! -> Do not confuse this with the pixels holding 4 float values for color 
	td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; // I want to sample this in a shader, hence shader resource.
	td.CPUAccessFlags = 0; // I want to be able to edit the texture when the mouse is being pressed. -> Writing data to a specific position
	td.MiscFlags = 0; // No miscellaneous information to give.
	td.Usage = D3D11_USAGE_DEFAULT; // I want to be able to edit the texture when the mouse is being pressed. -> Writing data to a specific position
	td.Format = DXGI_FORMAT_R32_FLOAT; // UNORM means unsigned normalized integers -> normalized means 0-255 becomes a 0-1 value, however the value we provide is from 0-255
	// To indicate no multi-sampling, SampleDesc.Count = 1 & SampleDesc.Quality = 0
	td.SampleDesc.Count = 1; // Number of multi-samples per pixel
	td.SampleDesc.Quality = 0; // The higher the quality level, the worse the performance


	// -- CREATE TEXTURE DATA : CPU SIDE --

	// Every pixel will hold 4 color values, with each color value - R G B A - being 8 bits long. 
	// 1 unsigned char -> 8 bits (1 byte)
	// 4 values for color -> 2 unsigned char

	int size = width * height; // total number of pixels
	previousData = new float[size * channelSize]; // multiplied by 4 for rgba color channels
	currentData = new float[size * channelSize];
	float alphaVal;
	int index;
	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			index = (i * width);
			index += (j);
			index *= channelSize;
			// Set color 
			// R
			//currentData[index] = (unsigned char)alphaVal;
			//previousData[index] = (unsigned char)0;
			// G 
			//currentData[index + 1] = (unsigned char)0;
			//previousData[index + 1] = (unsigned char)0;
			// B
			//currentData[index + 2] = (unsigned char)0;
			//previousData[index + 2] = (unsigned char)0;
			
			// A
			currentData[index] = (float)0;
			previousData[index] = (float)0;


		}
	}

	// -- CREATE TEXTURE BUFFERS -- 
	
	// We want to feed in this initial data to initialize the buffer
	D3D11_SUBRESOURCE_DATA preLoadData = {};// Create a SUBRESOURCE_DATA object. 
	preLoadData.pSysMem = previousData; // What structure is holding the data to be loaded?
	preLoadData.SysMemPitch = dataTypeSize * width * channelSize; // Since textures are to be read in two dimensions, what is the size of one row?
	Graphics::Device->CreateTexture2D(&td, &preLoadData, previous.GetAddressOf());

	// Change the pointer to the data structure before creating the "current" texture buffer
	preLoadData.pSysMem = currentData;
	Graphics::Device->CreateTexture2D(&td, &preLoadData, current.GetAddressOf());

	// Now we can delete the data structures on the CPU side
	delete[] previousData;
	delete[] currentData;
	previousData = nullptr;
	currentData = nullptr;
	

	// Great! The texture data is now stored as a buffer on the GPU, and the CPU side has been cleaned out.
	// However, the shaders will still not access the texture buffers directly. 
	
	// To provide more security and control, shaders need to be explicitly told 
	// 1) the number of mipmap levels they can access
	// 2) the number of array elements (in the case of a texture array) they can access
	// This is done by a SHADER RESOURCE VIEW -> How am I supposed to view this shader resource?

	// -- CREATE SHADER RESOUURCE VIEWS (SRV) -- 
	
	// In most cases, the entire resource should be accessed
	// This is also the default state of a UAV description.
	// Thus, we can pass 0 or NULL for the UAV description pointer.
	// However, I will specify a shader resource view here

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = td.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	Graphics::Device->CreateShaderResourceView(previous.Get(), &srvDesc, previousSRV.GetAddressOf());

	// Note, every resource must have its own shader resource view.
	// These cannot be shared, because even though two buffers (say textures) have the same structure, 
	// THEY DO NOT HOLD THE SAME DATA. 
	
	Graphics::Device->CreateShaderResourceView(current.Get(), &srvDesc, currentSRV.GetAddressOf());

	// -- CREATE UNORDERED ACCES VIEWS (UAV) --
	// An unordered access view allows resources to be read from and written to simultaneously. 
	// In my ripple setup, at any given time, one of the two textures needs to be sampled and written to simultaneously. 
	// However, the textures swap roles every frame, so a UAV must be created for each frame. 

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = td.MipLevels - 1;
	Graphics::Device->CreateUnorderedAccessView(previous.Get(), &uavDesc, previousUAV.GetAddressOf());
	Graphics::Device->CreateUnorderedAccessView(current.Get(), &uavDesc, currentUAV.GetAddressOf());

	v1[0] = currentUAV.Get();
	v1[1] = previousUAV.Get();
	v1[2] = currentUAV.Get();

	// -- BOOLEAN FOR SWAPPING BUFFERS --
	// -- COLOR OF RIPPLE --
	isCurrent = true;
	alphaVals = new float[1];
	alphaVals[0] = (float)4500.0f;
	
}

void BackgroundRipple::LoadSampler() 
{
	// great! 
	// The textures have been created (stored as Texture2D buffers on the GPU)
	// The instructions for the shaders for accessing the buffers are also stored on the GPU (as SRVs)
	
	// Now, the instructions for loading a color of a pixel from a texture must be defined -> this is known as sampling
	// Sampling is performed using UV coordinates
	// In HLSL, UV of top left is 0,0 -> UV of bottom right is 1,1
	// UV coordinates -> the opposite corner of 0,0 is 1,1
	
	// There are two main criteria for sampling
	// 1) Address mode : What should the shader do if the UV value of a pixel is outside the 0 - 1 UV range?
	// 2) Filter : What should the shader do if the UV value falls right between two pixels?

	// Three kinds of filters : Point filtering, linear filtering, anisotropic filtering
	// Three stages of filtering : Minification, Magnification, Mipmap reference
	// Address modes : Border, Clamp, Wrap, Mirror, Mirror Once
	
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[3] = 0.0f; // Setting alpha of the border color to 0
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Graphics::Device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
}



void BackgroundRipple::Update(float deltaTime) 
{
	frameX = Input::GetMouseX();
	frameY = Input::GetMouseY();
	if (Input::MouseLeftPress() && frameX > 1 && frameX < width - 1 && frameY > 1 && frameY < height - 1)
	{
		frameX--;
		frameY--;
		printf("Pressed\n");
		printf("%i\n", frameX);
		printf("%i\n", frameY);

		

		D3D11_BOX box = {};
		box.back = 1;
		box.front = 0;
		box.left = frameX;
		box.right = box.left+1;
		box.top = frameY;
		box.bottom = box.top+1;

		// https://eatplayhate.me/2013/09/29/d3d11-texture-update-costs/
		if (isCurrent) 
		{
			Graphics::Context->UpdateSubresource(current.Get(), 0, &box, &alphaVals[0], channelSize * dataTypeSize * width, height * channelSize * dataTypeSize * width);
		}
		else
		{
			Graphics::Context->UpdateSubresource(previous.Get(), 0, &box, &alphaVals[0], channelSize * dataTypeSize * width, height * channelSize * dataTypeSize * width);
		}
	}

	// Update Ripple
	UpdateBuffers();
}

void BackgroundRipple::UpdateBuffers()
{
	if (timePassed < frameLength) { return; }

	// Want to run pixel shader for updating itself and its neighbours. 
	// Drawing to a different render target. 

	// Set correct viewport?
		
	// -- SET SHADERS -- 
	{
		Graphics::Context->PSSetShader(calcPixelShader.Get(), 0, 0);
	}

	// -- WRITE TO AND BIND CONSTANT BUFFER --
	{
		// Binding buffer -> OLD
		// Graphics::Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		
		constData.screenSize.x = Window::Width();
		constData.screenSize.y = Window::Height();

		Graphics::FillAndBindNextConstantBuffer(&constData, sizeof(RippleData), D3D11_VERTEX_SHADER, 0);
	}

	// -- SET SHADER RESOURCES & SAMPLER STATE--
	{
		if (isCurrent)
		{
			Graphics::Context->OMSetRenderTargetsAndUnorderedAccessViews(0, NULL, NULL, 1, 2, &v1[0], NULL);
			
		}
		else
		{
			Graphics::Context->OMSetRenderTargetsAndUnorderedAccessViews(0, NULL, NULL, 1, 2, &v1[1], NULL);
			
		}
	}


	// -- DISABLE DEPTH BUFFER --
	// -- SET OUTPUT MERGER STATE / BLEND STATE --
	{
		Graphics::DisableZBuffer();
		Graphics::Context->OMSetBlendState(blendState.Get(), NULL, 0xffffff);
	}

	// -- DRAW TRIANGLES BASED ON INDICES --
	Graphics::Context->DrawIndexed(indexCount, 0, 0);
	
	isCurrent = !isCurrent;
	timePassed = 0;

	// -- RE-ENABLE Z-BUFFER --
	// -- RESET RENDER TARGET --
	Graphics::EnableZBuffer();
	ID3D11UnorderedAccessView* nullUAV[2] = {nullptr, nullptr};
	Graphics::Context->OMSetRenderTargetsAndUnorderedAccessViews(1, Graphics::BackBufferRTV.GetAddressOf(), Graphics::DepthBufferDSV.Get(), 1, 2, nullUAV, NULL);
}

void BackgroundRipple::Draw(float deltaTime) 
{	
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
		Graphics::Context->PSSetShader(displayPixelShader.Get(), 0, 0);
	}

	// -- BIND VERTEX AND INDEX BUFFERS --
	{
		UINT stride = sizeof(Vertex); // How big is one vertex of this particular object?
		UINT offset = 0; // Start from the beginning or some bytes after?

		Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		Graphics::Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	// -- WRITE TO AND BIND CONSTANT BUFFER --
	{
		// Binding buffer -> OLD
		// Graphics::Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		constData.screenSize.x = Window::Width();
		constData.screenSize.y = Window::Height();
		
		Graphics::FillAndBindNextConstantBuffer(&constData, sizeof(RippleData), D3D11_VERTEX_SHADER, 0);
	}

	// -- SET SHADER RESOURCES & SAMPLER STATE--
	{
		if (!isCurrent) 
		{
			Graphics::Context->PSSetShaderResources(0, 1, currentSRV.GetAddressOf());
		}
		else 
		{
			Graphics::Context->PSSetShaderResources(0, 1, previousSRV.GetAddressOf());
		}
		
		Graphics::Context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	}

	
	// -- DISABLE DEPTH BUFFER --
	// -- SET OUTPUT MERGER STATE / BLEND STATE --
	{
		Graphics::DisableZBuffer();
		Graphics::Context->OMSetBlendState(blendState.Get(), NULL, 0xffffff);
	}

	// -- DRAW TRIANGLES BASED ON INDICES --
	Graphics::Context->DrawIndexed(indexCount, 0, 0);
	Graphics::Context->PSSetShaderResources(0, 1, unbindPS);

	// -- RE-ENABLE Z-BUFFER --
	Graphics::EnableZBuffer();

	timePassed += deltaTime;
}