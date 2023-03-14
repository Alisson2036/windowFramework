#include "inputLayout.h"

InputLayout::InputLayout()
{

	//getDevice(gfx)->CreateInputLayout()
}

void InputLayout::bind(Graphics& gfx)
{
	getContext(gfx)->IASetPrimitiveTopology(topology);
	getContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
