#include "ShaderInclude.hlsli"

cbuffer CirclePSConstants : register(b0)
{
    float2 radiusXY;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float c = distance(input.center, input.screenPosition.xy);
    c = smoothstep(0.2, 0.3, 1 - c / radiusXY.x);
    input.color *= c;
    
    
    return float4(c, 0.0f, 0.0f, 1.0f);

}