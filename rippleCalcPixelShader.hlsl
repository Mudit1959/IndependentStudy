#include "ShaderInclude.hlsli"

Texture2D prevFrame : register(t0);
RWTexture2D<float4> nextFrame : register(u2);
SamplerState thisSampler : register(s0);

void main(RippleVertexToPixel input)
{
    float2 uv = input.uv;
    int2 backgroundSize = input.backgroundSize;
    
    /*
    float xIncrement = 1.0f / (float) backgroundSize.x;
    float yIncrement = 1.0f / (float) backgroundSize.y;
    
    float2 left = uv - float2(xIncrement, 0);
    float2 right = uv + float2(xIncrement, 0);
    float2 up = uv + float2(0, yIncrement);
    float2 down = uv - float2(0, yIncrement);
    */
    
    int2 index = int2(floor(uv.x * backgroundSize.x - 1), floor(uv.y * backgroundSize.y - 1));
    if (index.x < 1 || index.y < 1 || uv.y > 0.999 || uv.x > 0.999)
    {
        nextFrame[index] = float4(1, 1, 1, 1);
    }
    else
    {
        nextFrame[index] = prevFrame[index];
    }
   
    
    
    
    // Indexing is from 0 - backgroundSize - 1
    
    
}