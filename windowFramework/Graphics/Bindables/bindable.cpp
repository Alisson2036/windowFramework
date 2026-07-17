#include "bindable.h"



ID3D11Device* device;
ID3D11DeviceContext* context;

void Bindable::setDevice(ID3D11Device* _device)
{
	device = _device;
}

void Bindable::setContext(ID3D11DeviceContext* _deviceContext)
{
	context = _deviceContext;
}

ID3D11Device* Bindable::getDevice()
{
	return device;
}

ID3D11DeviceContext* Bindable::getContext()
{
	return context;
}

bool Bindable::isInitialized()
{
	return initialized;
}
