#pragma once
#include <DirectXMath.h>
#include "v.h"
#include "object.h"

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "vertexConstantBuffer.h"


#include "timer.h"


class Triangle : public Object
{
public:

	void create(vertex2d vertices[3])
	{

		vs.setDevice(device.Get());
		vs.setContext(context.Get());
		il.setDevice(device.Get());
		il.setContext(context.Get());
		ps.setDevice(device.Get());
		ps.setContext(context.Get());
		vb.setDevice(device.Get());
		vb.setContext(context.Get());
		ib.setDevice(device.Get());
		ib.setContext(context.Get());
		cvb.setDevice(device.Get());
		cvb.setContext(context.Get());



		vs.create(L"VertexShader.cso");

		ps.create(L"PixelShader.cso");

		il.create(vs,
			{
				{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			},
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);



		vb.create(
			vertices,
			3,
			sizeof(vertex2d)
		);

		short indexes[3] = { 0,1,2 };

		ib.create(
			indexes,
			3
		);


		addBindable(&cvb);
		addBindable(&ps);
		addBindable(&vs);
		addBindable(&vb);
		addBindable(&il);
		addBindable(&ib);

		timeSinceCreation.reset();

		indicesNum = 3;
		isIndexedBool = true;
	}

	void update() override 
	{
		float angle = timeSinceCreation.getPassedSeconds();
		DirectX::XMMATRIX b[] = {
			DirectX::XMMatrixScaling(3.0f/4.0f,1.0f,1.0f) *
			DirectX::XMMatrixRotationZ(angle)
		};

		cvb.create(
			b,
			1,
			sizeof(b)
		);
	}
	void draw() override
	{
	}

private:
	Timer timeSinceCreation;

	VertexShader vs;
	PixelShader ps;
	InputLayout il;
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

};
