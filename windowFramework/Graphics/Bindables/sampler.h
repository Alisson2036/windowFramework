#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "bindable.h"


class Sampler : public Bindable
{
public:

	void create(bool isAntialiased, bool wrap);

	void bind() override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

};