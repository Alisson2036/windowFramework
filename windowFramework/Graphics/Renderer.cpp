#include "Renderer.h"

Renderer::Renderer(ID3D11DeviceContext* context, VertexBufferCache* vbCache)
{
	m_context = context;
	m_vbCache = vbCache;

	// Creating instances buffer
	m_instancesBuffer.push_back(StructuredBuffer<DirectX::XMMATRIX>());
	m_instancesBuffer[0].create(nullptr, 128);
}

void Renderer::setObjects(std::span<RenderObject> bufferSpan)
{
	m_objectBuffer = bufferSpan;
}
void Renderer::execute(IRenderPass& pass)
{
	// Render pass bind
	pass.bind(m_context);

	const size_t capacity = m_instancesBuffer[0].getArraySize();
	if (capacity == 0 || m_objectBuffer.empty()) return; // Early exit

	std::vector<DirectX::XMMATRIX> tempInstBuffer;
	tempInstBuffer.reserve(capacity);

	// Batching
	const RenderObject* batchRepresentative = nullptr;

	for (auto& renderObject : m_objectBuffer)
	{
		// Culling: if obj needs to be culled, just continue

		// Check if it is the same batch
		bool isSameBatch = batchRepresentative &&
			batchRepresentative->sortKey == renderObject.sortKey;

		// Execute batch if type changed
		if (!isSameBatch || tempInstBuffer.size() == capacity)
		{
			if (!tempInstBuffer.empty())
			{
				executeBatch(tempInstBuffer, *batchRepresentative);
				tempInstBuffer.clear();
			}
			batchRepresentative = &renderObject;
		}

		// Push current object
		tempInstBuffer.push_back(renderObject.transformation.getMatrix());
	}

	// Flush remaining instances
	if (!tempInstBuffer.empty() && batchRepresentative)
	{
		executeBatch(tempInstBuffer, *batchRepresentative);
	}
}

void Renderer::executeBatch(std::vector<DirectX::XMMATRIX>& instances, RenderObject renderObject)
{

	// VertexBuffer from cache
	auto buffer = m_vbCache->getBuffer(
		renderObject.mesh,
		renderObject.material->getShader()
	);

	// Binds
	buffer->vBuffer.bind();     // VertexBuffer
	renderObject.material->bindMaterial(); // Shader and textures


	// Calcs the amount of instances to draw in this batch
	const UINT amount = instances.size();

	// Updates instances buffer
	m_instancesBuffer[0].update(instances.data(), amount);

	// Binding buffer
	m_instancesBuffer[0].bind();

	// Drawing
	if (buffer->iBuffer.isInitialized())
	{
		buffer->iBuffer.bind();
		m_context->DrawIndexedInstanced(buffer->vCount, amount, 0, 0, 0);
	}
	else
		m_context->DrawInstanced(buffer->vCount, amount, 0, 0);

}

void Renderer::createNewInstancesBuffer()
{
	StructuredBuffer<DirectX::XMMATRIX> newBuffer;
	newBuffer.create(nullptr, 128u);
	m_instancesBuffer.push_back(std::move(newBuffer));
}
