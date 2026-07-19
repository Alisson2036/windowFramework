#include "ShadowPass.h"

ShadowMapPass::ShadowMapPass()
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

RenderPassMask ShadowMapPass::getRenderMaskFilter()
{
	return RenderPassMask::ShadowMap;
}

void ShadowMapPass::setTarget(depthStencil* ds)
{
	targetDS = ds;
}

void ShadowMapPass::setLight(Light* light)
{
	m_light = light;
}

void ShadowMapPass::bind(ID3D11DeviceContext* context)
{
	Camera* camera = m_light->getCamera();
	// Preparacao camera buffers
	if (camera)
	{
		DirectX::XMMATRIX a = camera->getMatrix();
		cameraConstantBuffer.update(&a);
		DirectX::XMVECTOR b[] = { camera->getPositionVector() };
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
	if (targetDS) targetDS->bind();
	else _throwMsg("Target is not provided for shadowmap rendering.");

	ID3D11DepthStencilView* dsv = targetDS->getView();

	context->OMSetRenderTargets(
		0,
		nullptr,
		dsv
	);

}
