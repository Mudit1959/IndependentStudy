#include "ShaderInclude.hlsli"

Texture2D buffer : register(t0);
SamplerState thisSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    float2 alphaVals = buffer.Sample(thisSampler, input.uv).rg;
    float3 finalColor = float3(alphaVals.x, 0.0, 0.0) + (float3(1.0, 1.0, 1.0) * alphaVals.y);
    return float4(finalColor, 1.0f);
}