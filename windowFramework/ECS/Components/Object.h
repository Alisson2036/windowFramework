#pragma once

#include "..\..\Assets\MeshAsset.h"
#include "..\..\Assets\MaterialAsset.h"

struct RenderMask {
    enum Layer : uint32_t {
        None = 0,
        Opaque = 1 << 0,
        CastShadows = 1 << 1,
        UI = 1 << 2
    };

    uint32_t flags = Opaque | CastShadows;
};

struct CMeshNonIndexed
{
	MaterialAsset* material;
	MeshAsset* mesh;
    RenderMask renderMask{};
};




