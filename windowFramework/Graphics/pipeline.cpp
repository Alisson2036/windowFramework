#include "pipeline.h"





Pipeline::Pipeline(
	Microsoft::WRL::ComPtr<ID3D11Device> _device, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context,
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBufferView,
	depthStencil* _backDSBuffer,
	vec2 _windowResolution
	)
{
	device = _device;
	context = _context;

	aliasedSampler.create(true, false);
	sampler.create(false, false);
	
	//cria projection matrix
	DirectX::XMMATRIX mat[] = { DirectX::XMMatrixIdentity() };//camera->getProjectionMatrix() };
	cameraConstantBuffer.create(mat, 1, sizeof(DirectX::XMMATRIX));
	cameraConstantBuffer.setSlot(1);
	//cria buffer para posicao da camera
	DirectX::XMVECTOR vec[] = { DirectX::XMVECTOR({0.0f,0.0f,0.0f,0.0f})};
	cameraPositionBuffer.create(vec, 1, sizeof(DirectX::XMVECTOR));
	cameraPositionBuffer.setSlot(1);
	//cria o blendState
	blendState.create();

	windowResolution = _windowResolution;

	backDSBuffer = _backDSBuffer;
	backBufferView = _backBufferView;
}



void Pipeline::drawObject(object& obj)
{
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

	//faz o bind dos shaders
	obj.pShader->bind();

	//faz o bind do vertex buffer e constant vertex buffer
	obj.getConstantVertexBuffer()->bind();
	obj.vb.bind();
	if (obj.vbInstance.isInitialized())
		obj.vbInstance.bind();
	

	//texturas
	for (auto current : obj.textures)
	{
		if (current.second->isAntialiased())
			aliasedSampler.bind();
		else
			sampler.bind();
		aliasedSampler.bind();
		current.second->setSlot(current.first);
		current.second->bind();
	}

	//coloca o blendState
	blendState.bind();


	//luzes..caso existirem
	if (light)
		light->bind(0, 2);
	
	if (obj.ib.isInitialized())
	{
		obj.ib.bind();
		if (obj.instanceCount)
			context->DrawIndexedInstanced(obj.indexes.size(), obj.instanceCount, 0u, 0u, 0u);
		else
			context->DrawIndexed(obj.indexes.size(), 0u, 0u);
	}
	else
		if (obj.instanceCount)
			context->DrawInstanced(obj.getVertexCount(), obj.instanceCount, 0u, 0u);
		else
			context->Draw(obj.getVertexCount(), 0);
}

void Pipeline::setLight(Light* _light)
{
	light = _light;
}

void Pipeline::setCamera(Camera* _camera)
{
	camera = _camera;
}

void Pipeline::setRenderTarget(renderTarget* target, depthStencil* dtTarget)
{
	currentRenderTarget = target;

	if(target)
		target->bind();
	if (dtTarget)
		dtTarget->bind();

	if(target && dtTarget)
		context->OMSetRenderTargets(1, target->getViewPointer(), dtTarget->getViewPointer());
	else if(target)
		context->OMSetRenderTargets(1, target->getViewPointer(), NULL);
	else
		context->OMSetRenderTargets(0, NULL, dtTarget->getViewPointer());
}

void Pipeline::drawToScreen()
{
	//configura viewport
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, windowResolution.x, windowResolution.y, 0.0f, 1.0f };
	context->RSSetViewports(1, &viewport);
	//bind depth stencil state na pipeline
	backDSBuffer->bind();
	//configura render target
	context->OMSetRenderTargets(1, backBufferView.GetAddressOf(), backDSBuffer->getViewPointer());
}

void Pipeline::fillScreen(float r, float g, float b)
{
	const float f[4] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(backBufferView.Get(), f);
	backDSBuffer->clear();
}

void Pipeline::fillScreen(color c)
{
	fillScreen(
		float(1.0f) * c.r / 255.f,
		float(1.0f) * c.g / 255.f,
		float(1.0f) * c.b / 255.f
		);
}

vec2 Pipeline::getWindowResolution() const
{
	return windowResolution;
}
