#include "ShaderInclude.hlsli"

// Special semantic - SV_TARGET -> Means whatever the output is, paint it onto the current render target. 
// The output of the pixel shader is a single color in float4 -> RGBA 
// This pixel shader takes in data being passed from the vertex shader. The structure of the data is defined by VertexToPixel. 
// Named main, because that is the entry point for the compiler. 

float4 main(VertexToPixel input) : SV_TARGET
{
    float c = input.uv.y;
    c *= 2;
    c = (1 - abs(c - 1)) * input.smoothIntensity;
    return float4(input.color.rgb, c);
}