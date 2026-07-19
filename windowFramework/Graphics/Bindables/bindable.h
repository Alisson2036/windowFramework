#pragma once
#include <wrl.h>
#include "..\..\Core\exception.h"
#include <d3d11.h>


class Bindable
{
public:
	virtual void bind() {};

	static void setDevice(ID3D11Device* _device);
	static void setContext(ID3D11DeviceContext* _deviceContext);

	bool isInitialized();
protected:

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContext();

	bool initialized = false;

};