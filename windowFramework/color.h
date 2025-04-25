#pragma once


class color
{
public:
	color() = default;
	color(char R, char G, char B, char A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}


	char r;
	char g;
	char b;
	char a;
};