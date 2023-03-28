#include "bindable.h"



Microsoft::WRL::ComPtr<ID3D11Device> device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

void Bindable::setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device)
{
	device = _device;
}

void Bindable::setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext)
{
	context = _deviceContext;
}

Microsoft::WRL::ComPtr<ID3D11Device> Bindable::getDevice()
{
	return device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Bindable::getContext()
{
	return context;
}

bool Bindable::isInitialized()
{
	return initialized;
}
