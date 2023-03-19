#pragma once
#include <wrl.h>
#include "exception.h"


class Bindable
{
public:
	virtual void bind() {};
	//virtual void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device) = 0;
	//virtual void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext) = 0;

	void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device)
	{
		device = _device;
	}
	void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext)
	{
		context = _deviceContext;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice()
	{
		return device;
	}
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>getContext()
	{
		return context;
	}
	bool isInitialized()
	{
		return initialized;
	}
protected:
	bool initialized;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
};