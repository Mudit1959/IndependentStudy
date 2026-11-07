#include "ShaderInclude.hlsli"

VertexToPixel main( VertexShaderInput input ) : SV_POSITION
{
	// There needs to be an ouput struct
    VertexToPixel output;
    
    output.screenPosition = input.localPosition;
    output.smoothIntensity = 0;
    output.color = 0;
    output.uv = input.uv;
    
    return output;
}