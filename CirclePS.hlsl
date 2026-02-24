#include "ShaderInclude.hlsli"

cbuffer CirclePSConstants : register(b0)
{
    float radius;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float c = distance(input.center, input.screenPosition.xy);
    c = smoothstep(0.045, 0.1, 1 - c/radius);
    input.color *= c;
    
    return input.color;
}