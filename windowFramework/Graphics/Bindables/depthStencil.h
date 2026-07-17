#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "..\..\Math\vec2.h"
#include "..\..\Core\exception.h"
#include "texture.h"
#include "bindable.h"


class depthStencil : public Bindable
{
public:
	void create(vec2 bufferResolution);

	void bind() override;

	void clear();

	ID3D11DepthStencilView* getView();
	Texture* getTexture();
	vec2 getResolution();

private:
	Texture texInterface;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	vec2 resolution;

};

