#include "Renderer.h"

void Renderer::setObjects(std::span<RenderObject> bufferSpan)
{
	objectBuffer = bufferSpan;
}
