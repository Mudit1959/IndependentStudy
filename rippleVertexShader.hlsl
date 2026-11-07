#include "ShaderInclude.hlsli"

cbuffer RippleData : register(b0)
{
    float2 screenSize;
    float2 backgroundSize;
}

VertexToPixel main( VertexShaderInput input )
{
	// There needs to be an ouput struct
    VertexToPixel output;
    
    float4 finalPos = float4(input.localPosition, 1.0f);
    
    // Calculate the increment of one pixel one screen both along the X and Y axes
    float xOnePixel = 2.0f / screenSize.x;
    float yOnePixel = 2.0f / screenSize.y;
    
    // Scaling needs to take place in two stages. 
	// The first is to scale the box according to the final pixel length and height we want. 
	// Note that the vertices provided to the vertex shader must all have their coordinates and their uvs
	// to be scaled to a unit square. Otherwise, this will not work.
	
    finalPos.x *= backgroundSize.x;
    finalPos.y *= backgroundSize.y;
    
    // Once the points have been shifted, we now apply the single pixel ratio calculated for each axis 
	// to correctly map it on screen.
	// This is the 'second scaling'. - costed me 1 hour to figure out
    finalPos.x *= (xOnePixel);
    finalPos.y *= (yOnePixel);
    
    // Then Translate -> using the single pixel increments calculated before, translate the vertices
	// The vertices are translated from the top left corner of the screen, with positive X to the right, and positive Y going down.
    finalPos.x += -1;
    finalPos.y += 1;
    
    output.screenPosition = finalPos;
    output.smoothIntensity = 0;
    output.color = float4(22/25.0f, 22/25.0f, 22/25.0f, 0.5f);
    output.uv = input.uv;
    
    return output;
}