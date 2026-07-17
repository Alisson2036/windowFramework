#pragma once
#include "IRenderPass.h"
#include "RenderPassMask.h"
#include "..\Camera.h"
#include "..\light.h"
#include "..\Bindables\renderTarget.h"
#include "..\Bindables\depthStencil.h"


class ForwardRenderPass : public IRenderPass
{
public:
	ForwardRenderPass();

	RenderPassMask getRenderMaskFilter() override;

	void setTargets(renderTarget* rt, depthStencil* ds);
	void setLight(Light* light);
	void setCamera(Camera* camera);

	void bind(ID3D11DeviceContext* context) override;

private:
	Camera* m_camera;
	Light* m_light;

	// Camera buffers
	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;

	// Targets
	renderTarget* target;
	depthStencil* targetDS;

};