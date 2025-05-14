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


class renderTarget : private Bindable
{

public:
	void create(vec2 targetSize);


	Texture* getTexture();
	vec2 getResolution();

	void clear();
	void fill(float r, float g, float b);

	ID3D11RenderTargetView** getViewPointer();
	void bind() override;


private:
	Texture texInterface;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	vec2 targetResolution;

};