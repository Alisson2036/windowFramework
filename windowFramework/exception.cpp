#include "exception.h"

frameworkException::frameworkException(int _line, const char* _file)
	:
	file(_file),
	line(_line)
{
}

const char* frameworkException::what() const
{
	return file.c_str();
}

int frameworkException::getLine() const
{
	return line;
}

const char* frameworkException::getFile() const
{
	return file.c_str();
}
