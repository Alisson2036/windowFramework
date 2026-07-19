#include "ForwardRender.h"


ForwardRenderPass::ForwardRenderPass()
	:
	m_camera(nullptr),
	m_light(nullptr)
{

	//cria projection matrix
	DirectX::XMMATRIX mat[] = { DirectX::XMMatrixIdentity() };//camera->getProjectionMatrix() };
	cameraConstantBuffer.create(mat);
	cameraConstantBuffer.setSlot(1);

	//cria buffer para posicao da camera
	DirectX::XMVECTOR vec[] = { DirectX::XMVECTOR({0.0f,0.0f,0.0f,0.0f}) };
	cameraPositionBuffer.create(vec);
	cameraPositionBuffer.setSlot(1);

}

RenderPassMask ForwardRenderPass::getRenderMaskFilter()
{
	return RenderPassMask::Opaque;
}

void ForwardRenderPass::setTargets(renderTarget* rt, depthStencil* ds)
{
	target = rt;
	targetDS = ds;
}

void ForwardRenderPass::setLight(Light* light)
{
	m_light = light;
}

void ForwardRenderPass::setCamera(Camera* camera)
{
	m_camera = camera;
}

void ForwardRenderPass::bind(ID3D11DeviceContext* context)
{
	// Preparacao camera buffers
	if (m_camera)
	{
		DirectX::XMMATRIX a = m_camera->getMatrix();
		cameraConstantBuffer.update(a);
		DirectX::XMVECTOR b = m_camera->getPositionVector();
		cameraPositionBuffer.update(b);
	}
	else
		_throwMsg("Camera does not exist in the pipeline.");
	//bind projection matrix
	cameraConstantBuffer.bind();
	//bind camera position
	cameraPositionBuffer.bind();

	//luzes..caso existirem
	if (m_light)
		m_light->bind(0, 2);

	// Bind target
	if (target)   target->bind();
	if (targetDS) targetDS->bind();

	ID3D11RenderTargetView* rtv = target ? target->getView() : nullptr;
	ID3D11DepthStencilView* dsv = targetDS ? targetDS->getView() : nullptr;

	context->OMSetRenderTargets(
		rtv ? 1 : 0,
		rtv ? &rtv : nullptr,
		dsv
	);

}
