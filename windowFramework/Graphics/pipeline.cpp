#include "pipeline.h"





Pipeline::Pipeline(
	ID3D11Device* _device,
	ID3D11DeviceContext* _context,
	renderTarget* _backBuffer,
	Registry* _registry,
	VertexBufferCache* _vbCache,
	depthStencil* _backDSBuffer,
	vec2 _windowResolution
	)
	:
	device(_device),
	context(_context),
	camera(nullptr),
	currentRenderTarget(nullptr),
	light(nullptr),
	vbCache(_vbCache),
	registry(_registry),
	backDSBuffer(_backDSBuffer),
	backBuffer(_backBuffer),
	windowResolution(_windowResolution),
	renderer(context, vbCache),
	renderBuckets(sizeof(RenderPassMask)*8u)
{
	aliasedSampler.create(true, false);
	sampler.create(false, false);

	// Allocates render buckets
	
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


	// Cria o structured buffer para instancias, apenas para teste
	instancesBuffer.create(
		nullptr,
		128
	);
	instancesBuffer.setSlot(0);

}



void Pipeline::drawObject(Object& obj)
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
			context->DrawIndexedInstanced((UINT)obj.indexes.size(), obj.instanceCount, 0u, 0u, 0u);
		else
			context->DrawIndexed((UINT)obj.indexes.size(), 0u, 0u);
	}
	else
		if (obj.instanceCount)
			context->DrawInstanced((UINT)obj.getVertexCount(), obj.instanceCount, 0u, 0u);
		else
			context->Draw((UINT)obj.getVertexCount(), 0);
}

void Pipeline::drawScene(std::vector<IRenderPass*>&& renderPasses)
{
	Registry::View view = registry->getView<CMeshNonIndexed, SpatialData>();

	// Clears buffers
	for (auto& i : renderBuckets) i.clear();

	// Bucketing
	for (auto& obj : view) {
		CMeshNonIndexed* mesh = obj.get<CMeshNonIndexed>();
		SpatialData pos = *obj.get<SpatialData>();
		uint32_t mask = mesh->renderMask.flags;

		while (mask > 0) {
			// find bucket index
			size_t bucketIndex = std::countr_zero(mask);

			// Adding object
			renderBuckets[bucketIndex].push_back(
				Renderer::RenderObject{ mesh->material, mesh->mesh, pos }
			);

			// Next mask
			mask &= (mask - 1);
		}
	}

	// Vertex buffer hash functor
	VertexBufferCacheHash vbHash;

	// Array sorting
	for(size_t i = 0; i < renderBuckets.size(); i++)
	{
		std::sort(
			renderBuckets[i].begin(),
			renderBuckets[i].end(),
			[vbHash](const Renderer::RenderObject& a, const Renderer::RenderObject& b) {
				return vbHash({ a.mesh, a.material }) < vbHash({ b.mesh, b.material });
			}
		);
	}

	// Rendering passes
	for (auto pass : renderPasses)
	{
		uint32_t mask = pass->getRenderMaskFilter();
		size_t bucketIndex = std::countr_zero(mask);
		renderer.setObjects(renderBuckets[bucketIndex]);
		renderer.execute(*pass);

	}

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

	if (target)   target->bind();
	if (dtTarget) dtTarget->bind();

	ID3D11RenderTargetView* rtv = target ? target->getView() : nullptr;
	ID3D11DepthStencilView* dsv = dtTarget ? dtTarget->getView() : nullptr;

	context->OMSetRenderTargets(
		rtv ? 1 : 0,
		rtv ? &rtv : nullptr,
		dsv
	);
}

void Pipeline::drawToScreen()
{
	//configura viewport
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, windowResolution.x, windowResolution.y, 0.0f, 1.0f };
	context->RSSetViewports(1, &viewport);
	//bind depth stencil state na pipeline
	backDSBuffer->bind();
	//configura render target

	ID3D11RenderTargetView* rtv = backBuffer->getView();
	context->OMSetRenderTargets(1, &rtv, backDSBuffer->getView());
}

void Pipeline::fillScreen(float r, float g, float b)
{
	backBuffer->fill(r, g, b);
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

Registry* Pipeline::getRegistry() const
{
	return registry;
}
