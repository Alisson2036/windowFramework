#include "pipeline.h"


Microsoft::WRL::ComPtr<ID3D11Device>        device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;



Pipeline::Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
{
	device = _device;
	context = _context;

	colorBlend.initialize(
		L"ColorBlendVS.cso",
		L"ColorBlendPS.cso",
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
}

void Pipeline::bind()
{
	colorBlend.bind();
}

void Pipeline::staticBind::initialize(const wchar_t* vertexShader, const wchar_t* pixelShader, std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescription)
{
	vs.setContext(context);
	vs.setDevice(device);
	ps.setContext(context);
	ps.setDevice(device);
	il.setContext(context);
	il.setDevice(device);

	//CRIA PIXEL SHADER
	vs.create(vertexShader);

	//CRIA PIXEL SHADER
	ps.create(pixelShader);

	//CRIA INPUT LAYOUT
	il.create(&vs, elementDescription, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Pipeline::staticBind::bind()
{
	if (vs.isInitialized()) vs.bind();
	if (ps.isInitialized()) ps.bind();
	if (il.isInitialized()) il.bind();
}
