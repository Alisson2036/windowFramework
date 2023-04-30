#include "pipeline.h"





Pipeline::Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
{
	device = _device;
	context = _context;


	//colorBlend
	staticBinds.push_back(StaticBind(
		L"ColorBlendVS.cso",
		L"ColorBlendPS.cso"
	));

	staticBinds.push_back(StaticBind(
		L"texturedVS.cso",
		L"texturedPS.cso"
	));

	staticBinds.push_back(StaticBind(
		L"tex2dVS.cso",
		L"tex2dPS.cso"
	));

	sampler.create();

	//cria projection matrix
	DirectX::XMMATRIX mat[] = { Camera::getProjectionMatrix() };
	cameraConstantBuffer.create(mat, 1, sizeof(DirectX::XMMATRIX));
	cameraConstantBuffer.setSlot(1);
	//cria buffer para posicao da camera
	DirectX::XMVECTOR vec[] = { DirectX::XMVECTOR({0.0f,0.0f,0.0f,0.0f})};
	cameraPositionBuffer.create(vec, 1, sizeof(DirectX::XMVECTOR));
	cameraPositionBuffer.setSlot(1);
}


void Pipeline::initializeBindable(Bindable* bindable)
{
	bindable->setContext(context.Get());
	bindable->setDevice(device.Get());
}

void Pipeline::bind(ObjectDescriptor* desc)
{
	if (camera)
	{
		DirectX::XMMATRIX a[] = { camera->getMatrix() };
		cameraConstantBuffer.update(a);
		DirectX::XMVECTOR b[] = { camera->getPositionVector() };
		cameraPositionBuffer.update(b);
	}
	else
		_throwMsg("Camera does not exist in the pipeline.");
	//bind projection matrix
	cameraConstantBuffer.bind();
	//bind camera position
	cameraPositionBuffer.bind();

	//static binds
	if(lastBinded != desc->type) 
		staticBinds[desc->type].bind();
	lastBinded = desc->type;

	//dynamic binds
	if(desc->indexBuffer) desc->indexBuffer->bind();
	if(desc->constantVertexBuffer) desc->constantVertexBuffer->bind();
	if(desc->vertexBuffer) desc->vertexBuffer->bind();

	//texture
	if (!desc->texture.empty())
	{
		sampler.bind();
		for (auto i : desc->texture)
			i->bind();
	}
	

	//luzes..caso existirem
	if(light)
		light->bind(0);


	//desenha tudo
	if (desc->indexBuffer)
		context->DrawIndexed(desc->indicesNum, 0, 0);
	else
		context->Draw(desc->indicesNum, 0);
}

void Pipeline::setLight(Light* _light)
{
	light = _light;
}

void Pipeline::setCamera(Camera* _camera)
{
	camera = _camera;
}

Pipeline::StaticBind::StaticBind(const wchar_t* vertexShader, const wchar_t* pixelShader)
{
	//CRIA PIXEL SHADER
	vs.create(vertexShader);

	//CRIA PIXEL SHADER
	ps.create(pixelShader);

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	ShaderDesc sd(vs.getBlob());

	//checando os semantic names do vertex shader e criando o input layout correto
	for (std::string& semantic : sd.inputParams)
	{
		try
		{
			desc.push_back({ semantic.c_str(), 0, layouts.at(semantic), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		catch (...)
		{
			_throwMsg("Shader input semantic name doesn't match any input type");
		}
	}

	

	//CRIA INPUT LAYOUT
	il.create(&vs, desc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Pipeline::StaticBind::bind()
{
	if (vs.isInitialized()) vs.bind();
	if (ps.isInitialized()) ps.bind();
	if (il.isInitialized()) il.bind();
}
