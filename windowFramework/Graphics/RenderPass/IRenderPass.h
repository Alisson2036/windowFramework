#pragma once
#include <stdint.h>

#include "RenderPassMask.h"


class IRenderPass
{
public:
	virtual RenderPassMask getRenderMaskFilter() = 0;
	virtual ~IRenderPass() = 0;
	
	
	virtual void bind() = 0;
};