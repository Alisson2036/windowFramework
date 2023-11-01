#pragma once

class vec3
{
public:
	//construtores padrao
	vec3() = default;
	vec3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	vec3(vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}

	//operadores
	vec3 operator+(vec3 rhs) const
	{
		return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	vec3 operator-(vec3 rhs) const
	{
		return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	vec3 operator*(vec3 rhs) const
	{
		return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
	}
	vec3 operator/(vec3 rhs) const
	{
		return vec3(x / rhs.x, y / rhs.y, z / rhs.z);
	}
	vec3 operator+=(vec3 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	vec3 operator-=(vec3 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	vec3 operator*=(vec3 rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
	}
	vec3 operator/=(vec3 rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
	}




	float x;
	float y;
	float z;

};