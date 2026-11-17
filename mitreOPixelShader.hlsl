#include "ShaderInclude.hlsli"

SamplerState thisSampler : register(s0); // Basic Sampler

cbuffer MitreOData : register(b0)
{
    float flipX;
    float flipY;
}

float4 main(VertexToPixel data) : SV_TARGET
{
    float c = abs(data.uv.x - flipX);
	return float4(c, c, c, c);
}