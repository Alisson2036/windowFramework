#pragma once
#include <wrl.h>
#include <map>

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "vertexConstantBuffer.h"
#include "texture.h"
#include "sampler.h"
#include "shaderDesc.h"

#include "light.h"
#include "Instance.h"
#include "Camera.h"






class Pipeline
{
public:
	Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context);

	enum ObjectType
	{
		ColorBlend,
		Textured
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

	void setLight(Light* _light);
	void setCamera(Camera* _camera);

private:


	class StaticBind
	{
	public:
		//construtor para inicializar com VS, PS e IL
		StaticBind(const wchar_t* vertexShader, const wchar_t* pixelShader);
		//coloca na pipeline
		void bind();
	private:
		VertexShader vs;
		PixelShader ps;
		InputLayout il;

		std::map<std::string, DXGI_FORMAT> layouts =
		{
			{ "Position",  DXGI_FORMAT_R32G32B32_FLOAT       },
			{ "TexCoord",  DXGI_FORMAT_R32G32_FLOAT          },
			{ "Normals",   DXGI_FORMAT_R32G32B32_FLOAT       },
			{ "Color",     DXGI_FORMAT_R8G8B8A8_UNORM        }
		};
	};

private:

	Light* light;

	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	std::vector<StaticBind> staticBinds;

	Sampler sampler;

	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;
	Camera* camera;

	int lastBinded = -1;

};