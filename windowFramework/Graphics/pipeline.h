#pragma once
#include <wrl.h>
#include <map>

#include "Bindables\vertexShader.h"
#include "Bindables\pixelShader.h"
#include "Bindables\vertexBuffer.h"
#include "Bindables\inputLayout.h"
#include "Bindables\indexBuffer.h"
#include "Bindables\blendState.h"
#include "Bindables\vertexConstantBuffer.h"
#include "Bindables\texture.h"
#include "Bindables\sampler.h"
#include "Shader\shaderDesc.h"
#include "object.h"
#include "Bindables\renderTarget.h"
#include "Bindables\depthStencil.h"

#include "light.h"
#include "..\Math\SpatialData.h"
#include "Camera.h"
#include "..\Math\vec2.h"






class Pipeline
{
public:
	Pipeline(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, vec2 _windowResolution);


	void initializeBindable(Bindable* bindable);
	void drawObject(object& obj);

	void setLight(Light* _light);
	void setCamera(Camera* _camera);

	void setRenderTarget(renderTarget* target, depthStencil* dtTarget);

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

	renderTarget* currentRenderTarget;

	vec2 windowResolution;


};