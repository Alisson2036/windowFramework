#pragma once
#include <DirectXMath.h>
#include "..\Math\vec2.h"
#include "..\Math\vec3.h"


class Camera
{
public:
	Camera() = default;

	Camera(vec3 Position, vec2 Angle);

	void setPositionAndAngle(vec3 Position, vec2 Angle);

	void move(vec3 Position, vec2 Angle);
	void movePosition(vec3 Position);
	void moveAngle(vec2 Angle);

	void setScreenProportion(float proportion);
	void setViewSize(vec2 _viewSize);
	void setPerspective(bool enable);
	
	DirectX::XMMATRIX getMatrix();
	DirectX::XMVECTOR getPositionVector();

	DirectX::XMMATRIX getProjectionMatrix();



private:
	DirectX::XMMATRIX getRotationMatrix();

	vec3 position;
	vec2 angle;
	vec2 viewSize;

	float screenProportion = 1.0f;
	bool perspectiveEnable = true;

};

