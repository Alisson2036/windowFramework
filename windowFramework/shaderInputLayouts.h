#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <string>


namespace shaderInputLayouts
{

	struct inputElement
	{
		DXGI_FORMAT format;
	};
	std::map<std::string, DXGI_FORMAT> layouts =
	{
		{ "Position",  DXGI_FORMAT_R32G32B32_FLOAT       },
		{ "TexCoord",  DXGI_FORMAT_R32G32_FLOAT          },
		{ "Normals",   DXGI_FORMAT_R32G32B32_FLOAT       },
		{ "Color",     DXGI_FORMAT_R8G8B8A8_UNORM        }
	};
}