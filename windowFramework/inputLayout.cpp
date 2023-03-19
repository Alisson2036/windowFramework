#include "inputLayout.h"



void InputLayout::create(VertexShader& vs,std::vector<D3D11_INPUT_ELEMENT_DESC>elementDescription, D3D11_PRIMITIVE_TOPOLOGY _topology)
{
	topology = _topology;

	getDevice()->CreateInputLayout(
		elementDescription.data(),
		elementDescription.size(),
		vs.getBlob()->GetBufferPointer(),
		vs.getBlob()->GetBufferSize(),
		&pInputLayout
	);
	//initialized = true;
}

void InputLayout::bind()
{
	getContext()->IASetPrimitiveTopology(topology);
	getContext()->IASetInputLayout(pInputLayout.Get());
}
