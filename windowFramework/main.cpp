#include <Windows.h>
#include <string>
#include <exception>
#include "app.h"
#include "exception.h"

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
	catch (const frameworkException& e)
	{
		std::string error = "PROGRAM ERROR\n";
		error += e.what();
		error += '\n';
		error += "file:";
		error += e.getFile();
		error += "\nline: ";
		error += std::to_string(e.getLine());

		MessageBoxA(nullptr, error.c_str(), "FRAMEWORK EXCEPTION", MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		std::string error = e.what();
		MessageBoxA(nullptr, error.c_str(), "STD EXCEPTION", 0);
		
	}
	catch (...)
	{
		MessageBoxA(nullptr, "UNHANDLED EXCEPTION", "???", 0);
	}
	return 0;
}