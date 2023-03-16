#pragma once
#include <vector>
#include <memory>
#include "graphics.h"
#include "exception.h"
#include "bindable.h"


class Object
{
public:
	class Fill
	{
		friend Object;
	public:
		Fill(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
			:
			device(_device),
			context(_context)
		{}

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>        device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	};



	void bind()
	{
		for (auto i = 0; i < bindables.size(); i++)
		{
			bindables[i]->bind();
		}
	}
	bool isIndexed() { return isIndexedBool; }
	int indexNum() { return indicesNum; }
	
	virtual void update() {};
	virtual void draw() {};

	void addBindable(Bindable* bind)
	{
		bindables.push_back(bind);
	}

	void setFill(Fill filler)
	{
		device = filler.device;
		context = filler.context;
	}

	
	void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device)
	{
		device = _device;
	}
	void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext)
	{
		context = _deviceContext;
	}
	

protected:
	std::vector<Bindable*> bindables;


	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	bool isIndexedBool = false;
	int indicesNum = 0;


};