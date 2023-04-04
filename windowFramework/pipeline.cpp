#include "pipeline.h"





Pipeline::Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
{
	device = _device;
	context = _context;


	//colorBlend
	staticBinds.push_back(StaticBind(
		L"ColorBlendVS.cso",
		L"ColorBlendPS.cso",
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	));

	staticBinds.push_back(StaticBind(
		L"texturedVS.cso",
		L"texturedPS.cso",
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normals", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	));

	sampler.create();

	//cria projection matrix
	//DirectX::XMMATRIX mat[] = { Instance::getProjectionMatrix() };
	//projectionMatrixConstantBuffer.create(mat, 1, sizeof(DirectX::XMMATRIX));
	//projectionMatrixConstantBuffer.setSlot(1);
}


void Pipeline::initializeBindable(Bindable* bindable)
{
	bindable->setContext(context.Get());
	bindable->setDevice(device.Get());
}

void Pipeline::bind(ObjectDescriptor* desc)
{
	//bind projection matrix
	projectionMatrixConstantBuffer.bind();

	//static binds
	if(lastBinded != desc->type) 
		staticBinds[desc->type].bind();
	lastBinded = desc->type;

	//dynamic binds
	if(desc->indexBuffer) desc->indexBuffer->bind();
	if(desc->constantVertexBuffer) desc->constantVertexBuffer->bind();
	if(desc->vertexBuffer) desc->vertexBuffer->bind();

	//texture
	if (desc->texture)
	{
		sampler.bind();
		desc->texture->bind();
	}
	

	//luzes
	light.bind(2);

	if (desc->indexBuffer)
		context->DrawIndexed(desc->indicesNum, 0, 0);
	else
		context->Draw(desc->indicesNum, 0);
}

Pipeline::StaticBind::StaticBind(const wchar_t* vertexShader, const wchar_t* pixelShader, std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescription)
{
	//CRIA PIXEL SHADER
	vs.create(vertexShader);

	//CRIA PIXEL SHADER
	ps.create(pixelShader);

	//CRIA INPUT LAYOUT
	il.create(&vs, elementDescription, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Pipeline::StaticBind::bind()
{
	if (vs.isInitialized()) vs.bind();
	if (ps.isInitialized()) ps.bind();
	if (il.isInitialized()) il.bind();
}
