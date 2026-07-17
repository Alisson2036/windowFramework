#pragma once
#include <stdint.h>

#include "RenderPassMask.h"


class IRenderPass
{
public:
	virtual RenderPassMask getRenderMaskFilter() = 0;
	virtual ~IRenderPass() = default;
	
	
	virtual void bind() = 0;
};