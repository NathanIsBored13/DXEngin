#include "ExtendedException.h"

ExtendedException::ExtendedException(int line, const char* file, const char* type, HRESULT hr) : errorStr(GenerateErrorString(line, file, type, hr)) {}

const char* ExtendedException::what() const noexcept
{
	return errorStr.c_str();
}

std::string ExtendedException::GenerateErrorString(int line, const char* file, const char* type, HRESULT hr)
{
	std::stringstream oss;
	oss << type << std::endl << "[Error Code] " << hr << std::endl << "[Description] " << TranslateError(hr) << std::endl << "[File] " << file << std::endl << "[Line] " << line;;
	return oss.str();
}

std::string ExtendedException::TranslateError(HRESULT hr)
{
	char* pMsgBuffer = nullptr;
	DWORD msgLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);
	if (msgLength == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuffer;
	LocalFree(pMsgBuffer);
	return errorString;
}