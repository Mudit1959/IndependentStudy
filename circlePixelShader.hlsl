#include "ShaderInclude.hlsli"

// Special semantic - SV_TARGET -> Means whatever the output is, paint it onto the current render target. 
// The output of the pixel shader is a single color in float4 -> RGBA 
// This pixel shader takes in data being passed from the vertex shader. The structure of the data is defined by VertexToPixel. 
// Named main, because that is the entry point for the compiler. 


float4 main(VertexToPixel input) : SV_TARGET
{
    float r = distance(input.uv, float2(0.5, 0.5));
    float c = 1 - smoothstep(0.45, 0.5, r);
	return float4(input.color.rgb, c);
}