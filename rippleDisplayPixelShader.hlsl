#include "ShaderInclude.hlsli"

Texture2D<float> buffer : register(t0);
SamplerState thisSampler : register(s0);

float4 main(RippleVertexToPixel input) : SV_TARGET
{
    float a = clamp(buffer.Sample(thisSampler, input.uv), 0, 255);
    a /= 255.0f;
    // When sampling here, my alpha value is from 0 - 255
    return float4(a,a,a,a);
}