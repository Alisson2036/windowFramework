#pragma once
#include "IRenderPass.h"
#include "RenderPassMask.h"
#include "..\Camera.h"
#include "..\light.h"

class ForwardRenderPass : public IRenderPass
{
public:
	ForwardRenderPass();

	RenderPassMask getRenderMaskFilter() override;

	void setLight(Light* light);
	void setCamera(Camera* camera);

	void bind() override;

private:
	Camera* m_camera;
	Light* m_light;

	// Camera buffers
	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;

};