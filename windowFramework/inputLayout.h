#pragma once
#include "bindable.h"
#include "exception.h"


class InputLayout : Bindable
{
public:
	InputLayout();

	void bind(Graphics& gfx) override;



private:
	D3D11_PRIMITIVE_TOPOLOGY topology;
	D3D11_INPUT_ELEMENT_DESC elementDescription;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

};