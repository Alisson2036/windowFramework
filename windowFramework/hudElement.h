#pragma once
#include "object.h"


class hudElement
{
public:
	void create(Texture& tex);
	
	object obj;

private:
	shader shader2d;

};