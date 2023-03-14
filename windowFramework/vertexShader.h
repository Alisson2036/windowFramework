#pragma once
#include <wrl.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "exception.h"
#include "bindable.h"



class VertexShader : public Bindable
{
public:

	void create(const wchar_t* _shaderFile);

	void bind() override;

	Microsoft::WRL::ComPtr<ID3DBlob> getBlob();

private:


	const wchar_t* shaderFile;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>  vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob>            vertexShaderBlob;

};