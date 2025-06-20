#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "bindable.h"
#include "vertexShader.h"
#include "..\..\Core\exception.h"



class InputLayout : public Bindable
{
public:
	void create(VertexShader* vs, std::vector<D3D11_INPUT_ELEMENT_DESC>elementDescription, D3D11_PRIMITIVE_TOPOLOGY _topology);

	void bind() override;
	



private:
	D3D11_PRIMITIVE_TOPOLOGY topology{};
	//D3D11_INPUT_ELEMENT_DESC* elementDescription;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

};