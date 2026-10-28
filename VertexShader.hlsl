#include "ShaderInclude.hlsli"

cbuffer BoxData : register(b0)
{
    float2 offset;
    float2 screenSize;
	
    float2 boxSize;
    float opacity;
    float angle;
    
}

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{ 
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 localPosition	: POSITION;     // XYZ position
	float4 color			: COLOR;        // RGBA color
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;
	
	// At this point, we have the original quad
	// *--------* top left is the origin to rotate around 
	// |		|
	// |		|
	// *--------*
    float4 finalPos = float4(input.localPosition, 1.0f);
	
    // Calculate the increment of one pixel one screen both along the X and Y axes
    float xOnePixel = 2.0f / screenSize.x;
    float yOnePixel = 2.0f / screenSize.y;
    
	// Scaling needs to take place in two stages. 
	// The first is to scale the box according to the final pixel length and height we want. 
	// Note that the vertices provided in the vertex shader must all have a magnitude of 1, otherwise, this will not work as intended!
    float newX = finalPos.x * boxSize.x;
    float newY = finalPos.y * boxSize.y;
    
	// Treating each vertex as a vector, we rotate it. 
	// This can be derived mathematically using trignometric identities
	// sin(a +- b) = sin(a)cos(b) +- cos(a)sin(b)
	// cos(a +- b) = cos(a)cos(b) -+ sin(a)sin(b)
    finalPos.x = (newX * cos(angle)) + (newY * sin(angle));
    finalPos.y = (newY * cos(angle)) - (newX * sin(angle));
	
	// The s
    finalPos.x *= (xOnePixel);
    finalPos.y *= (yOnePixel);
    
	
	// Then Translate -> using the single pixel increments calculated before, translate the vertices
   
    finalPos.x += -1 + (xOnePixel * offset.x);
    finalPos.y += 1 - (yOnePixel * offset.y);

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in the future).
	output.screenPosition = finalPos;

	// Pass the color through 
	// - The values will be interpolated per-pixel by the rasterizer
	// - We don't need to alter it here, but we do need to send it to the pixel shader
    output.color = float4(input.color.xyz, opacity);

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}