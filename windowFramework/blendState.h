#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"
#include "bindable.h"


class BlendState : public Bindable
{
public:
	void create();

	void bind() override;

private:
	
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

};

