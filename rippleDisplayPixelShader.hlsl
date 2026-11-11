#include "ShaderInclude.hlsli"

Texture2D buffer : register(t0);
SamplerState thisSampler : register(s0);

float4 main(RippleVertexToPixel input) : SV_TARGET
{
    return buffer.Sample(thisSampler, input.uv).rgba;
}