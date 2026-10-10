#include "ShaderInclude.hlsli"

cbuffer BoxData : register(b0)
{
    float2 offset;
    uint2 screenSize;
    float2 boxSize;
    float opacity;
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
	
    float3 finalPos = input.localPosition.xyz;
    float xIncrement, yIncrement;
    xIncrement = 2 / (float) screenSize.x;
    yIncrement = 2 / (float) screenSize.y;
	
    finalPos.x = -1 + (boxSize.x * xIncrement * finalPos.x) + (offset.x * xIncrement);
    finalPos.y = 1 + (boxSize.y * yIncrement * finalPos.y) - (offset.y * yIncrement);

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in the future).
	output.screenPosition = float4(finalPos, 1.0f);

	// Pass the color through 
	// - The values will be interpolated per-pixel by the rasterizer
	// - We don't need to alter it here, but we do need to send it to the pixel shader
    output.color = float4(input.color.xyz, opacity);

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}