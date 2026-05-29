#include "Renderer.h"

Renderer::Renderer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, VertexBufferCache* vbCache)
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

void Renderer::execute()
{
	std::vector<DirectX::XMMATRIX> tempInstBuffer;
	tempInstBuffer.reserve(m_instancesBuffer[0].getArraySize());
	auto buffer = m_vbCache->getBuffer(
		m_objectBuffer[0].mesh,
		m_objectBuffer[0].material->getShader()
	);

	for (size_t i = 0; i < m_objectBuffer.size(); i++)
	{
		auto& renderObject = m_objectBuffer[i];

		tempInstBuffer.push_back(renderObject.transformation.getMatrix());

		// Checks if render can wait (batch is being made)
		auto type = m_vbHash({ m_objectBuffer[i].mesh, m_objectBuffer[i].material });
		if (tempInstBuffer.size() != tempInstBuffer.capacity() && i < m_objectBuffer.size() - 1) {
			auto& next = m_objectBuffer[i + 1];
			auto nextType = m_vbHash({ next.mesh, next.material });
			if (type == nextType)
				continue;
		}


		// VertexBuffer from cache
		auto buffer = m_vbCache->getBuffer(
			renderObject.mesh,
			renderObject.material->getShader()
		);

		// Binds
		buffer->vBuffer.bind();     // VertexBuffer
		renderObject.material->bindMaterial(); // Shader and textures


		// Calcs the amount of instances to draw in this batch
		const UINT amount = tempInstBuffer.size();

		// Updates instances buffer
		m_instancesBuffer[0].update(tempInstBuffer.data(), amount);

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

		// Resets tempInstBuffer
		tempInstBuffer.clear();
	}

}

void Renderer::createNewInstancesBuffer()
{
	StructuredBuffer<DirectX::XMMATRIX> newBuffer;
	newBuffer.create(nullptr, 128u);
	m_instancesBuffer.push_back(std::move(newBuffer));
}
