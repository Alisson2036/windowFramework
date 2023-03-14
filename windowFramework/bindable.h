#pragma once
#include "graphics.h"
#include "exception.h"


class Bindable
{
public:
	virtual void bind(Graphics& gfx) = 0;

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice(Graphics& gfx)
	{
		return gfx.getDevice();
	}
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext(Graphics& gfx)
	{
		return gfx.getContext();
	}

};