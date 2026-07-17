#pragma once
#include <stdint.h>
#include <d3d11.h>
#include "RenderPassMask.h"


class IRenderPass
{
public:
	virtual RenderPassMask getRenderMaskFilter() = 0;
	virtual ~IRenderPass() = default;
	
	
	virtual void bind(ID3D11DeviceContext* context) = 0;
};