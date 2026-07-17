#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "bindable.h"
#include "depthStencil.h"
#include "..\..\Core\exception.h"
#include "texture.h"
#include "..\..\Math\vec2.h"


class renderTarget : private Bindable
{

public:
	void create(vec2 targetSize);
	void create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);

	Texture* getTexture();
	vec2 getResolution();

	void clear();
	void fill(float r, float g, float b);

	ID3D11RenderTargetView* getView();
	void bind() override;

private:
	void create_internal(D3D11_TEXTURE2D_DESC texDesc, Microsoft::WRL::ComPtr<ID3D11Texture2D>& pTexture);

private:
	Texture texInterface;
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	vec2 targetResolution;

};