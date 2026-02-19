#include "ShaderInclude.hlsli"

cbuffer RoundedRectConstants : register(b0)
{
    float rounding      : ROUND_RADIUS_PIXELS;
    float2 widthHeight  : WIDTH_HEIGHT_PIXELS;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    //float c = distance(input.center, input.screenPosition.xy);
    //c = smoothstep(0.2, 0.3, 1 - c / rounding.x);
    //input.color *= c;
    
    // Taking screenPosition in pixels as (x,y) on graph
    // Editing the center to make the bottom of the rect the x-axis
    float2 origin = float2(input.center.x, input.center.y - widthHeight.y);
    
    
    return input.color;
}