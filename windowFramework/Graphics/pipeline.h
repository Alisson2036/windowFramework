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
#include "..\ECS\Registry.h"
#include "..\ECS\Components\Object.h"
#include "Caching\VertexBufferCache.h"
#include "Shader\shaderDesc.h"
#include "object.h"
#include "Bindables\renderTarget.h"
#include "Bindables\depthStencil.h"

#include "light.h"
#include "..\Math\SpatialData.h"
#include "Camera.h"
#include "..\Math\vec2.h"

// Temporario
#include "Bindables\structuredBuffer.h"


class Pipeline
{
public:
	Pipeline(
		Microsoft::WRL::ComPtr<ID3D11Device> _device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBufferView,
		Registry* _registry,
		VertexBufferCache* _vbCache,
		depthStencil* _backDSBuffer,
		vec2 _windowResolution
	);


	void drawObject(object& obj);
	void drawScene();

	// --Setters--

	void setLight(Light* _light);
	void setCamera(Camera* _camera);
	void setRenderTarget(renderTarget* target, depthStencil* dtTarget);

	// Define renderTarget como o BackBuffer da janela
	void drawToScreen();

	void fillScreen(float r, float g, float b);
	void fillScreen(color c);

	vec2 getWindowResolution() const;
	Registry* getRegistry() const;


private:


	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// Back buffer e depth stencil da janela

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferView;
	depthStencil* backDSBuffer;

	// Samplers
	Sampler aliasedSampler;
	Sampler sampler;

	// Camera buffers
	ConstantVertexBuffer cameraConstantBuffer;
	ConstantPixelBuffer cameraPositionBuffer;

	// RCS vars
	Registry* registry;
	ConstantVertexBuffer transformBuffer;
	StructuredBuffer<DirectX::XMMATRIX> instancesBuffer;
	
	// Caching
	VertexBufferCache* vbCache;


	BlendState blendState;
	Camera* camera;
	Light* light;

	renderTarget* currentRenderTarget;

	vec2 windowResolution;


};