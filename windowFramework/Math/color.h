#pragma once


class color
{
public:
	color() = default;
	color(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}


	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};