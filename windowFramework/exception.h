#pragma once
#include <Windows.h>
#include <exception>
#include <string>


class frameworkException : public std::exception
{
public:
	frameworkException(int _line, const char* _file);
	frameworkException(int _line, const char* _file, HRESULT _hr);

	const char* what() const override;


	int getLine() const;
	const char* getFile() const;

	

private:

	std::string file;
	int line;
	HRESULT hr;

};

#define _throw throw frameworkException(__LINE__, __FILE__)
#define _throwHr(hr) if(FAILED(hr)) throw frameworkException(__LINE__, __FILE__, hr)