#pragma once
#include <vector>
#include <span>

#include "RenderPass\IRenderPass.h"
#include "Caching\VertexBufferCache.h"
#include "..\Assets\MaterialAsset.h"
#include "..\Math\SpatialData.h"
#include "Bindables\structuredBuffer.h"

class Renderer
{
public:
	Renderer(
		ID3D11DeviceContext* context,
		VertexBufferCache* vbCache
	);

	struct RenderObject {
		MaterialAsset* material;
		MeshAsset* mesh;
		SpatialData transformation;
		uint64_t sortKey;
	};

	void setObjects(std::span<RenderObject> bufferSpan);

	void execute(IRenderPass& pass);

private:
	struct PipelineState {
		MaterialAsset* material;
		VertexBufferCache::Buffer* vertexInfo;
		
		size_t instanceBufferIndex; // Which instance buffer to use
		size_t instanceBufferOffset; // Offset inside this instance buffer to send to the shader

		UINT instanceCount;
	};
	void executeBatch(std::vector<DirectX::XMMATRIX>& instances, RenderObject renderObject);

	void createNewInstancesBuffer();

private:

	ID3D11DeviceContext* m_context;

	std::span<RenderObject> m_objectBuffer;

	VertexBufferCache* m_vbCache;
	VertexBufferCacheHash m_vbHash;

	std::vector<StructuredBuffer<DirectX::XMMATRIX>> m_instancesBuffer;
};