#pragma once
#include "..\..\Assets\MeshAsset.h"
#include "..\..\Assets\ShaderAsset.h"
#include "..\Bindables\vertexBuffer.h"
#include "..\Bindables\indexBuffer.h"



struct VertexBufferCacheHash {
    std::uint64_t operator()(const std::pair<IAsset*, IAsset*>& p) const {
        return (static_cast<uint64_t>(p.first->getId())) << 32 | 
            static_cast<uint64_t>(p.second->getId());
    }
};


class VertexBufferCache
{
public:
    struct Buffer
    {
        IndexBuffer iBuffer;
        VertexBuffer vBuffer;
        UINT vCount=0;
    };


    Buffer* addBuffer(MeshAsset* mesh, ShaderAsset* shader);

    Buffer* getBuffer(MeshAsset* mesh, ShaderAsset* shader);

private:
    std::unordered_map <
        std::pair<MeshAsset*, ShaderAsset*>,
        Buffer,
        VertexBufferCacheHash
    > cacheMap = {};

};


