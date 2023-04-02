#pragma once
#include <wrl.h>

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "vertexConstantBuffer.h"
#include "texture.h"
#include "sampler.h"

class Pipeline
{
public:
	Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context);

	enum ObjectType
	{
		ColorBlend
	};

	struct ObjectDescriptor
	{
		ObjectType type;
		IndexBuffer* indexBuffer;
		VertexBuffer* vertexBuffer;
		ConstantVertexBuffer* constantVertexBuffer;
		Texture* texture;
		int indicesNum;
	};

	void initializeBindable(Bindable* bindable);
	void bind(ObjectDescriptor* desc);

private:


	class StaticBind
	{
	public:
		//construtor para inicializar com VS, PS e IL
		StaticBind(const wchar_t* vertexShader, const wchar_t* pixelShader, std::vector<D3D11_INPUT_ELEMENT_DESC>elementDescription);
		//coloca na pipeline
		void bind();
	private:
		VertexShader vs;
		PixelShader ps;
		InputLayout il;
	};

private:

	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	std::vector<StaticBind> staticBinds;

	Sampler sampler;

	int lastBinded = -1;

};