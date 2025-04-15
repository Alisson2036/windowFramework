#pragma once
#include <map>
#include "vertexShader.h"
#include "pixelShader.h"
#include "inputLayout.h"
#include "inputBuffer.h"
#include "shaderDesc.h"


class shader
{
public:
	//apaga outros construtores 
	shader(shader&) = delete;
 	
	//construtor para inicializar com VS, PS e IL
	shader() = default;
	shader(const wchar_t* vertexShader, const wchar_t* pixelShader);
	void create(const wchar_t* vertexShader, const wchar_t* pixelShader);

	//coloca na pipeline
	void bind();


	std::vector<inputBuffer::type> inputParams;
private:
	bool initialized = false;

	VertexShader vs;
	PixelShader ps;
	InputLayout il;

	struct format
	{
		DXGI_FORMAT format;
		char size;
	};

	const std::map<std::string, format> layouts =
	{
		{ "Position",  {DXGI_FORMAT_R32G32B32_FLOAT, 12    }},
		{ "TexCoord",  {DXGI_FORMAT_R32G32_FLOAT   ,  8    }},
		{ "Normals",   {DXGI_FORMAT_R32G32B32_FLOAT, 12    }},
		{ "Tangents",   {DXGI_FORMAT_R32G32B32_FLOAT, 12    }},
		{ "Color",     {DXGI_FORMAT_R8G8B8A8_UNORM ,  4    }},
		{ "Position2d",{DXGI_FORMAT_R32G32_FLOAT   ,  8    }}
	};
};