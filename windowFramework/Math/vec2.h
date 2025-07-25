#pragma once



class vec2
{
public:
	//construtores padrao
	vec2() = default;
	vec2(const float X, const float Y)
	{
		x = X;
		y = Y;
	}
	vec2(const vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	
	//operadores
	vec2 operator+(vec2 rhs) const
	{
		return vec2(x + rhs.x, y + rhs.y);
	}
	vec2 operator-(vec2 rhs) const
	{
		return vec2(x - rhs.x, y - rhs.y);
	}
	vec2 operator*(vec2 rhs) const
	{
		return vec2(x * rhs.x, y * rhs.y);
	}
	vec2 operator/(vec2 rhs) const
	{
		return vec2(x / rhs.x, y / rhs.y);
	}
	vec2 operator+=(vec2 rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	vec2 operator-=(vec2 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	vec2 operator*=(vec2 rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	vec2 operator/=(vec2 rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
	}
	



	float x;
	float y;
};