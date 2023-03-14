#pragma once
#include <wrl.h>
#include "exception.h"


class Bindable
{
public:
	

	virtual void bind() = 0;
	virtual void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device) = 0;
	virtual void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext) = 0;


	Microsoft::WRL::ComPtr<ID3D11Device> getDevice()
	{
		return device;
	}
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>getContext()
	{
		return context;
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
};