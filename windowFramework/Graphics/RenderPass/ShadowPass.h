#pragma once
#include "IRenderPass.h"
#include "RenderPassMask.h"
#include "..\Camera.h"
#include "..\light.h"
#include "..\Bindables\renderTarget.h"
#include "..\Bindables\depthStencil.h"


class ShadowMapPass : public IRenderPass
{
public:
	ShadowMapPass();

	RenderPassMask getRenderMaskFilter() override;

	void setTarget(depthStencil* ds);
	void setLight(Light* light);

	void bind(ID3D11DeviceContext* context) override;

private:
	Light* m_light;

	// Camera buffers
	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;

	// Targets
	depthStencil* targetDS;

};