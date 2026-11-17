#include "ShaderInclude.hlsli"

RWTexture2D<float> prevFrame : register(u1);
RWTexture2D<float> nextFrame : register(u2);

// background size should be passed as a contant here, rather than being assigned to each vertex and interpolated!
// same should go for color

void main(RippleVertexToPixel input)
{
    float2 uv = input.uv;
    int2 backgroundSize = input.backgroundSize;
    float value = 0;
    
    int2 index = int2(floor(uv.x * (backgroundSize.x - 1)), floor(uv.y * (backgroundSize.y - 1)));
    index = clamp(index, int2(1, 1), int2(backgroundSize.x - 2, backgroundSize.y - 2));
    
    int2 right,left, up, down;
    
    right = left = up = down = index;
   
    right.x += 1;
    left.x -= 1;
    up.y += 1;
    down.y -= 1;
    
    value = prevFrame[left] + prevFrame[right] + prevFrame[up] + prevFrame[down];
    value = value/2.0f;
    // different viable damping intensities
    // 0.9759 - 0.9859 - 0.9899 - 0.9959 - 0.9999
    value *= 0.98590f;
    value = value - nextFrame[index];
    
    nextFrame[index] = value;
    
    
    
    
}