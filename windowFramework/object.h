#pragma once
#include <vector>
#include <memory>
#include "graphics.h"
#include "exception.h"
#include "bindable.h"

#include "pipeline.h"


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
			if (bindables[i]->isInitialized())
				bindables[i]->bind();
			else
				_throwMsg("Bindable not initialized trying to bind");
		}
	}
	bool isIndexed() { return isIndexedBool; }
	int indexNum() { return indicesNum; }
	
	virtual void update() {};
	void draw() 
	{
		//pipeline.bind();
		bind();
		if (isIndexedBool)
			context->DrawIndexed(indicesNum, 0, 0);
		else
			context->Draw(indicesNum, 0);
	};

	void addBindable(Bindable* bind)
	{
		bindables.push_back(bind);
	}

	void setFill(Fill filler)
	{
		device = filler.device;
		context = filler.context;
		//pipeline.create(device, context);
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
	void fillBindables()
	{
		for (auto bind : bindables)
		{
			bind->setContext(context.Get());
			bind->setDevice(device.Get());
		}
	}

	std::vector<Bindable*> bindables;

	//Pipeline pipeline;


	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	bool isIndexedBool = false;
	int indicesNum = 0;


};