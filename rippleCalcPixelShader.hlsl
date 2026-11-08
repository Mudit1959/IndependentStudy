#include "ShaderInclude.hlsli"

Texture2D previousBuffer : register(t0);
Texture2D currentBuffer : register(t1);
SamplerState thisSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    if (input.uv.r == 0 || input.uv.g == 0)
    {
        return float4(currentBuffer.Sample(thisSampler, input.uv).rg, 0, 0);
    }
    
    float2 currentVals = currentBuffer.Sample(thisSampler, input.uv).rg;
    float2 previousVals = previousBuffer.Sample(thisSampler, input.uv).rg;
    
    float uIncrement = 1 / input.backgroundSize.x;
    float vIncrement = 1 / input.backgroundSize.y;
    
    float2 left, right, top, bottom;
    left = input.uv - float2(uIncrement, 0);
    right = input.uv + float2(uIncrement, 0);
    top = input.uv - float2(0, vIncrement);
    bottom = input.uv + float2(0, vIncrement);
    
    float acc = previousBuffer.Sample(thisSampler, left).g;
    acc += previousBuffer.Sample(thisSampler, right).g;
    acc += previousBuffer.Sample(thisSampler, top).g;
    acc += previousBuffer.Sample(thisSampler, bottom).g;
    acc /= 2.0f;
    currentVals.g *= -1;
    currentVals.g += acc;
    
    return float4(currentVals.rg, 0, 0);
}