#include "pipeline.h"

void Pipeline::create(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
{
	device = _device;
	context = _context;

	vs.setContext(context);
	vs.setDevice(device);
	ps.setContext(context);
	ps.setDevice(device);
	il.setContext(context);
	il.setDevice(device);


	//CRIA PIXEL SHADER
	vs.create(L"colorBlendVS.cso");

	//CRIA PIXEL SHADER
	ps.create(L"colorBlendPS.cso");

	//CRIA INPUT LAYOUT
	il.create(&vs,
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		},
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

}

void Pipeline::bind()
{
	vs.bind();
	ps.bind();
	il.bind();
}
