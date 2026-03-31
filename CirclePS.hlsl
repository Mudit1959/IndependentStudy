#include "ShaderInclude.hlsli"

// Using signed distance field logic for analytical shapes as explained on proceduralpixels.com - SDF
/*
float SDF(float2 vec, float radius)
{
    return length(vec) - radius;
}


float sd = SDF(input.screenPosition.xy - input.center, radius+1);
    float pixelWidth = fwidth(sd);
    float alpha = smoothstep(pixelWidth, 0, sd);

*/

cbuffer CirclePSConstants : register(b0)
{
    float radius;
}


float4 main(VertexToPixel input) : SV_TARGET
{
    
    float c = distance(input.center, input.screenPosition.xy);
    c = smoothstep(0, 0.1, 1 - c / radius);
    input.color *= c;
    
    return input.color;
}

