#include "exception.h"

frameworkException::frameworkException(int _line, const char* _file)
	:
	file(_file),
	line(_line),
	hr(0),
	msg("")
{
}

frameworkException::frameworkException(int _line, const char* _file, HRESULT _hr)
	:
	file(_file),
	line(_line),
	hr(_hr),
	msg("")
{
}

frameworkException::frameworkException(int _line, const char* _file, std::string _message)
	:
	file(_file),
	line(_line),
	hr(0),
	msg(_message)
{
}

const char* frameworkException::what() const
{
	if (FAILED(hr))
	{
		
		LPSTR message = nullptr;
		if (FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&message,
			0,
			nullptr
		) != 0) return message;

		return "couldn't find message";

	}
	return msg.c_str();
}

int frameworkException::getLine() const
{
	return line;
}

const char* frameworkException::getFile() const
{
	return file.c_str();
}
