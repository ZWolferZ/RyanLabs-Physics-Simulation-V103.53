#pragma once
#pragma region Includes
// Include{s}
#include <windows.h>
#include <string>
#pragma endregion
class Debug
{
public:

	
	static void Debug_WriteString(const std::string& message)
	{
		OutputDebugStringA((message + "\n").c_str());
	}

	static void Debug_WriteVector(const std::string& objectType, const std::string& vectorType, const Vector& vector)
	{
		std::string message = objectType + " - " + vectorType + " - X: " + std::to_string(vector.x) + ", Y: " +
			std::to_string(vector.y) + ", Z: " + std::to_string(vector.z) + "\n";
		OutputDebugStringA(message.c_str());
	}

	static void Debug_WriteVector(const std::string& objectType, const std::string& vectorType,
		const DirectX::XMFLOAT3& vector)
	{
		std::string message = objectType + " - " + vectorType + " - X: " + std::to_string(vector.x) + ", Y: " +
			std::to_string(vector.y) + ", Z: " + std::to_string(vector.z) + "\n";
		OutputDebugStringA(message.c_str());
	}

	static void Debug_WriteNumber(const std::string& string, const float& number)
	{
		std::string message = string + std::to_string(number) + "\n";
		OutputDebugStringA(message.c_str());
	}

	static int VDebugPrintF(const char* format, va_list args)
	{
		constexpr UINT32 MAX_CHARS = 1024;
		static char s_buffer[MAX_CHARS];

		int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, args);
		OutputDebugStringA(s_buffer);

		return charsWritten;
	}

	static int DebugPrintF(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		int charsWritten = VDebugPrintF(format, argList);
		va_end(argList);

		return charsWritten;
	}
};
