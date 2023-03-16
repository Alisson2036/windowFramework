#pragma once
#include "v.h"
#include "object.h"

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"

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
			{ indexes },
			3
		);


		addBindable(&ps);
		addBindable(&vs);
		addBindable(&vb);
		addBindable(&il);
		addBindable(&ib);
	}

	void update() override {}
	void draw() override
	{
	}

private:
	VertexShader vs;
	PixelShader ps;
	InputLayout il;
	VertexBuffer vb;
	IndexBuffer ib;

};
