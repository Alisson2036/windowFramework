#include "VertexBufferCache.h"

VertexBufferCache::Buffer* VertexBufferCache::addBuffer(MeshAsset* mesh, ShaderAsset* shader)
{
    std::pair pair(mesh, shader);

    // Exits if key already exists
    auto iter = cacheMap.find(pair);
    if (iter != cacheMap.end()) return &iter->second;

    // Creates buffer
    inputBuffer ib;
    ib.create(shader->getShader()->inputParams);
    mesh->getData(&ib);
    ib.createVertexBuffer(cacheMap[pair].vBuffer);
    cacheMap[pair].vCount = ib.getElementCount();
    return &cacheMap[pair];
}

VertexBufferCache::Buffer* VertexBufferCache::getBuffer(MeshAsset* mesh, ShaderAsset* shader)
{
    auto iter = cacheMap.find(std::pair(mesh, shader));
    if (iter == cacheMap.end()) return addBuffer(mesh, shader);

    return &iter->second;
}
