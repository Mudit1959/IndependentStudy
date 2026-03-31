#include "Graphics.h"

inline void DisableDepthBuffer()
{
	// Disable depth buffer
	Graphics::Context->OMSetRenderTargets(
		1,
		Graphics::BackBufferRTV.GetAddressOf(),
		0);
}

inline void RebindDepthBuffer()
{

	// Rebind depth buffer
	Graphics::Context->OMSetRenderTargets(
		1,
		Graphics::BackBufferRTV.GetAddressOf(),
		Graphics::DepthBufferDSV.Get());
}