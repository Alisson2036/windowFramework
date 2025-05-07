#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "vec2.h"
#include "exception.h"
#include "bindable.h"


class depthStencil : public Bindable
{
public:
	void create(vec2 bufferResolution);

	void bind() override;

	void clear();

	ID3D11DepthStencilView* getViewPointer();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	vec2 resolution;

};

