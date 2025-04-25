#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "bindable.h"
#include "image.h"
#include "exception.h"
#include "vec2.h"


class Texture : public Bindable
{
public:

	void create(Image& img, bool hasAntiAliasing=true);
	void createWithMipMap(Image& img, bool hasAntiAliasing = true);

	void update(Image& img);

	bool isAntialiased();

	void setSlot(unsigned short slot);

	void bind() override;

	vec2 getResolution();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	bool antialiased = true;
	unsigned short textureSlot = 0u;
	vec2 resolution;

};