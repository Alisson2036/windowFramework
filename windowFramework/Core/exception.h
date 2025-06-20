#pragma once
#include <Windows.h>
#include <exception>
#include <string>


class frameworkException : public std::exception
{
public:
	frameworkException(int _line, const char* _file);
	frameworkException(int _line, const char* _file, HRESULT _hr);
	frameworkException(int _line, const char* _file, std::string _message);

	const char* what() const override;


	int getLine() const;
	const char* getFile() const;

	

private:

	std::string msg;
	std::string file;
	int line;
	HRESULT hr;

};

#define _throw throw frameworkException(__LINE__, __FILE__)
#define _throwHr(hr) if(FAILED(hr)) throw frameworkException(__LINE__, __FILE__, hr)
#define _throwMsg(msg) throw frameworkException(__LINE__, __FILE__, msg)