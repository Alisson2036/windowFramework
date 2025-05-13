#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "bindable.h"
#include "depthStencil.h"
#include "exception.h"
#include "texture.h"
#include "vec2.h"


class targetView : private Bindable
{

public:
	void create(vec2 targetSize, bool enableDepthStencil);


	Texture* getTexture();
	vec2 getResolution();

	void clear();
	void fill(float r, float g, float b);


	void bind() override;


private:
	Texture texInterface;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	
	depthStencil depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	vec2 targetResolution;

};