#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "bindable.h"
#include "vertexShader.h"
#include "exception.h"
#include "texture.h"
#include "vec2.h"


class targetView : private Bindable
{
	friend class Graphics;

public:
	void create(vec2 targetSize, bool enableDepthStencil);


	Texture* getTexture();

	void clear();


private:
	void bind() override;
	Texture buffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	vec2 targetResolution;

};