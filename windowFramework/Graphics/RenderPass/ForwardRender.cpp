#include "ForwardRender.h"

ForwardRenderPass::ForwardRenderPass()
	:
	m_camera(nullptr),
	m_light(nullptr)
{

	//cria projection matrix
	DirectX::XMMATRIX mat[] = { DirectX::XMMatrixIdentity() };//camera->getProjectionMatrix() };
	cameraConstantBuffer.create(mat, 1, sizeof(DirectX::XMMATRIX));
	cameraConstantBuffer.setSlot(1);

	//cria buffer para posicao da camera
	DirectX::XMVECTOR vec[] = { DirectX::XMVECTOR({0.0f,0.0f,0.0f,0.0f}) };
	cameraPositionBuffer.create(vec, 1, sizeof(DirectX::XMVECTOR));
	cameraPositionBuffer.setSlot(1);
}

RenderPassMask ForwardRenderPass::getRenderMaskFilter()
{
	return RenderPassMask::Opaque;
}

void ForwardRenderPass::setLight(Light* light)
{
	m_light = light;
}

void ForwardRenderPass::setCamera(Camera* camera)
{
	m_camera = camera;
}

void ForwardRenderPass::bind()
{
	// Preparacao camera buffers
	if (m_camera)
	{
		DirectX::XMMATRIX a = m_camera->getMatrix();
		cameraConstantBuffer.update(&a);
		DirectX::XMVECTOR b[] = { m_camera->getPositionVector() };
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

}
