#pragma once
#include <vector>
#include <memory>
#include "graphics.h"
#include "exception.h"
#include "bindable.h"


class object
{
public:
	void bind(Graphics& gfx)
	{
		for (auto i = 0; i != bindables.size(); i++)
		{
			bindables[i]->bind(gfx);
		}
	}
	
	virtual void update() = 0;

	virtual void draw() = 0;

	void addBindable(Bindable& bind)
	{
		bindables.push_back(std::make_unique<Bindable>(bind));
	}
protected:
	std::vector<std::unique_ptr<Bindable>> bindables;


};