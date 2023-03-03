#include <Windows.h>
#include <string>
#include <exception>
#include "app.h"

int CALLBACK WinMain(
	_In_      HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_      LPSTR lpCmdLine,
	_In_      int nCmdShow
	)
{
	try
	{
		app main;
		main.start();
		
	}
	catch (const std::exception& a)
	{
		std::string error = a.what();
		MessageBoxA(nullptr, error.c_str(), "STD EXCEPTION", 0);
		
	}
	catch (...)
	{
		MessageBoxA(nullptr, "UNHANDLED EXCEPTION", "???", 0);
	}
	return 0;
}