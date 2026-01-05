#include "VertexBufferCache.h"

VertexBufferCache::Buffer* VertexBufferCache::addBuffer(MeshAsset* mesh, ShaderAsset* shader)
{
    std::pair pair(mesh, shader);

    // Exits if key already exists
    auto iter = cacheMap.find(pair);
    if (iter != cacheMap.end()) return &iter->second;

    // Creates vertex Buffer
    inputBuffer db;
    db.create(shader->getShader()->inputParams);
    mesh->getData(&db);
    db.createVertexBuffer(cacheMap[pair].vBuffer);
    cacheMap[pair].vCount = db.getElementCount();

    // Creates index buffer
    auto& ind = mesh->getIndices();
    if (ind.size() > 0u)
    {
        cacheMap[pair].iBuffer.create(ind.data(), ind.size());
        cacheMap[pair].vCount = ind.size();
    }
    // Returns buffer
    return &cacheMap[pair];
}

VertexBufferCache::Buffer* VertexBufferCache::getBuffer(MeshAsset* mesh, ShaderAsset* shader)
{
    auto iter = cacheMap.find(std::pair(mesh, shader));
    if (iter == cacheMap.end()) return addBuffer(mesh, shader);

    return &iter->second;
}
