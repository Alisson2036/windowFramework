#pragma once
#include <stdint.h>


enum RenderPassMask : uint32_t{
	Null = 0,
	Opaque = 1 << 0,
	ShadowMap = 1 << 1
};