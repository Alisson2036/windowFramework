#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "bindable.h"
#include "image.h"
#include "exception.h"


class Texture : public Bindable
{
public:

	void create(Image& img, bool hasAntiAliasing=true);

	bool isAntialiased();

	void bind() override;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	bool antialiased = true;

};