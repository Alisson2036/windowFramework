#pragma once
#include <wrl.h>
#include <map>

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "blendState.h"
#include "vertexConstantBuffer.h"
#include "texture.h"
#include "sampler.h"
#include "shaderDesc.h"
#include "object.h"

#include "light.h"
#include "SpatialData.h"
#include "Camera.h"
#include "vec2.h"






class Pipeline
{
public:
	Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, vec2 _windowResolution);


	void initializeBindable(Bindable* bindable);
	void bind(object& obj);

	void setLight(Light* _light);
	void setCamera(Camera* _camera);

	vec2 getWindowResolution() const;


private:

	Light* light;

	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;


	Sampler aliasedSampler;
	Sampler sampler;

	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;
	BlendState blendState;
	Camera* camera;

	vec2 windowResolution;


};