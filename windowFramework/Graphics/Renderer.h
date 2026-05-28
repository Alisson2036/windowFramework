#pragma once
#include <vector>
#include <span>

#include "Caching\VertexBufferCache.h"
#include "..\Assets\MaterialAsset.h"
#include "..\Math\SpatialData.h"
#include "Bindables\structuredBuffer.h"

class Renderer
{
public:
	Renderer() = default;

	struct RenderObject {
		MaterialAsset* material;
		MeshAsset* mesh;
		SpatialData transformation;
	};

	// Clears buffer but memory stays allocated
	void clearBuffer();

	// Frees memory used for buffer
	void eraseBuffer();

	void setObjects(std::span<RenderObject> bufferSpan);

	void execute();
private:
	struct PipelineState {
		MaterialAsset* material;
		VertexBufferCache::Buffer* vertexInfo;
		
		size_t instanceBufferIndex; // Which instance buffer to use
		size_t instanceBufferOffset; // Offset inside this instance buffer to send to the shader

		UINT instanceCount;
	};
	void bufferSetup();
	void executeOperations();


private:
	std::span<RenderObject> objectBuffer;

	std::vector<StructuredBuffer<DirectX::XMMATRIX>> instancesBuffer;
};