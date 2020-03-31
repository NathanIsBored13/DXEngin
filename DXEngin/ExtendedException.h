#pragma once

#include "Includes.h"

#include <sstream>
#include <string>

#define EE_DEFAULT_EXCEPTION(hr) ExtendedException(__LINE__, __FILE__, "DX3DEngine exception" ,hr)
#define EE_DEFAULT_EXCEPTION_GLE() ExtendedException(__LINE__, __FILE__, "DX3DEngine exception" , GetLastError())
#define EE_MSG_LOOP_EXCEPTION(hr) ExtendedException(__LINE__, __FILE__, "DX3DEngine message loop exception" ,hr)
#define EE_MSG_LOOP_EXCEPTION_GLE() ExtendedException(__LINE__, __FILE__, "DX3DEngine message loop exception" , GetLastError())
#define EE_WINDOW_EXCEPTION(hr) ExtendedException(__LINE__, __FILE__, "DX3DEngine window exception" ,hr)
#define EE_WINDOW_EXCEPTION_GLE() ExtendedException(__LINE__, __FILE__, "DX3DEngine window exception" , GetLastError())

class ExtendedException : public std::exception
{
public:
	ExtendedException(int line, const char* file, const char* type, HRESULT hr);
	const char* what() const noexcept;
	static std::string TranslateError(HRESULT hr);
private:
	std::string GenerateErrorString(int line, const char* file, const char* type, HRESULT hr);
	std::string errorStr;
};