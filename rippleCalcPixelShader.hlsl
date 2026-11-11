#include "ShaderInclude.hlsli"

RWTexture2D<float> prevFrame : register(u2);
RWTexture2D<float> nextFrame : register(u3);
SamplerState thisSampler : register(s0);

void main(RippleVertexToPixel input)
{
    float2 uv = input.uv;
    int2 backgroundSize = input.backgroundSize;
    float value = 0;
    
    int2 index = int2(floor(uv.x * (backgroundSize.x - 1)), floor(uv.y * (backgroundSize.y - 1)));
    index.x = clamp(index.x, 1, backgroundSize.x-2);
    index.y = clamp(index.y, 1, backgroundSize.y - 2);
    
    int2 right,left, up, down;
    
    right = left = up = down = index;
   
    right.x += 1;
    left.x -= 1;
    up.y += 1;
    down.y -= 1;
    
    value = prevFrame[left] + prevFrame[right] + prevFrame[up] + prevFrame[down];
    value /= 2.0f;
    
    nextFrame[index] = max(value * 0.84, 0);
    
    
    
    
}