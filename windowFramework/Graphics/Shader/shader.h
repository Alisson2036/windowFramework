#pragma once
#include <unordered_map>
#include <wrl.h>
#include "..\Bindables\vertexShader.h"
#include "..\Bindables\pixelShader.h"
#include "..\Bindables\inputLayout.h"
#include "inputBuffer.h"
#include "shaderDesc.h"


class Shader
{
public:
	//apaga outros construtores 
	Shader(Shader&) = delete;
 	
	//construtor para inicializar com VS, PS e IL
	Shader() = default;
	Shader(const wchar_t* vertexShader, const wchar_t* pixelShader);
	void create(const wchar_t* vertexShader, const wchar_t* pixelShader);
	void create(Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, Microsoft::WRL::ComPtr<ID3DBlob> pixelShader);

	//coloca na pipeline
	void bind();

	bool isInitialized();

	bool hasPerInstanceData();
	std::vector<inputBuffer::type> inputParams;

private:
	void initializeInputInfo();

private:
	bool initialized = false;
	bool hasInstancedData = false;


	VertexShader vs;
	PixelShader ps;
	InputLayout il;

	struct format
	{
		DXGI_FORMAT format;
		unsigned char size;
		bool perVertex;
	};

	const std::unordered_map<std::string, format> layouts =
	{
		{ "Position",  {DXGI_FORMAT_R32G32B32_FLOAT, 12, true  }},
		{ "TexCoord",  {DXGI_FORMAT_R32G32_FLOAT   ,  8, true  }},
		{ "Normals",   {DXGI_FORMAT_R32G32B32_FLOAT, 12, true  }},
		{ "Tangents",  {DXGI_FORMAT_R32G32B32_FLOAT, 12, true  }},
		{ "Color",     {DXGI_FORMAT_R8G8B8A8_UNORM ,  4, true  }},
		{ "Position2d",{DXGI_FORMAT_R32G32_FLOAT   ,  8, true  }},
		{ "InstPos",   {DXGI_FORMAT_R32G32B32_FLOAT, 12, false }}
	};
};