#pragma once
#include <exception>
#include <string>


class frameworkException : public std::exception
{
public:
	frameworkException(int _line, const char* _file);

	const char* what() const override;

	int getLine() const;
	const char* getFile() const;

private:
	std::string file;
	int line;

};