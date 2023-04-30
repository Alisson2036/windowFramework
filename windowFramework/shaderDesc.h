#pragma once
#include <wrl.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include "exception.h"

class ShaderDesc
{
public:
	ShaderDesc(Microsoft::WRL::ComPtr<ID3DBlob> blob);


	std::vector<std::string> inputParams;
private:


};