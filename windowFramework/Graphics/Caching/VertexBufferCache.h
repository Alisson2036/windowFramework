#pragma once
#include "..\..\Assets\MeshAsset.h"
#include "..\..\Assets\ShaderAsset.h"
#include "..\Bindables\vertexBuffer.h"



struct VertexBufferCacheHash {
    std::size_t operator()(const std::pair<MeshAsset*, ShaderAsset*>& p) const {
        auto h1 = std::hash<MeshAsset*>{}(p.first);
        auto h2 = std::hash<ShaderAsset*>{}(p.second);

        // Hash combination formula
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};


class VertexBufferCache
{
public:
    struct Buffer
    {
        VertexBuffer vBuffer;
        int vCount;
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


