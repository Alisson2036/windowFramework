#pragma once
#include <wrl.h>
#include "exception.h"
#include <d3d11.h>


class Bindable
{
public:
	virtual void bind() {};
	//virtual void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device) = 0;
	//virtual void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext) = 0;

	static void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device);
	static void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext);

	bool isInitialized();
protected:

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext();

	bool initialized = false;

};