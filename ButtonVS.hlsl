#include "ShaderInclude.hlsli"

//
cbuffer VSConstants : register(b0)
{
    float4x4 world : WORLD_MATRIX;
    float4x4 worldInvT : WORLD_INVERSE_MATRIX;
    float4x4 view : VIEW_MATRIX;
    float4x4 proj : PROJECTION_MATRIX;
	
    float2 objectWH;
    int2 screenWH;
    float cameraZ;
}

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output struct
    VertexToPixel output;

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in the future).
	
    //input.localPosition -= float4(1.0f, -1.0f, 0.0f, 0.0f);
	
    float4x4 wvp = mul(proj, mul(view, world));
    
	
    input.localPosition.x /= screenWH.x;
    input.localPosition.y /= screenWH.y;
	
    input.localPosition.x *= objectWH.x * 2;
    input.localPosition.y *= objectWH.y * 2;
	
    input.localPosition.x -= 1;
    input.localPosition.y += 1;
	
    output.screenPosition = float4(input.localPosition, 1.0f);
    

	// Pass the color through 
	// - The values will be interpolated per-pixel by the rasterizer
	// - We don't need to alter it here, but we do need to send it to the pixel shader
    output.color = input.color;
	
	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
    return output;
}