#include "ShaderInclude.hlsli"

SamplerState thisSampler : register(s0); // Basic Sampler

cbuffer CornerOData : register(b0)
{
    float flipX;
    float flipY;
}

float4 main(VertexToPixel data) : SV_TARGET
{
    return float4(abs(data.uv.x - flipX), abs(data.uv.y - flipY), 0, 1.0f);
}