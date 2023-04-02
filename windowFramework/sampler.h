#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "bindable.h"


class Sampler : public Bindable
{
public:

	void create();

	void bind() override;

private:


};