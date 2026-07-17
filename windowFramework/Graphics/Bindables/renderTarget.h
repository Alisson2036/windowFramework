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

	ID3D11RenderTargetView** getViewPointer();
	void bind() override;

private:
	void create_internal(D3D11_TEXTURE2D_DESC texDesc);

private:
	Texture texInterface;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	vec2 targetResolution;

};