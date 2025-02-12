#pragma once

#pragma region Includes
// Include{s}
#include <windows.h>
#include <string>
#pragma endregion

/// <summary>
/// Debug class for logging messages to the output window.
/// </summary>
class Debug
{
public:
#pragma region String Logging
	/// <summary>
	/// Allows the user to write a string message to the output window.
	/// </summary>
	/// <param name="message">The string message or sentence.</param>
	static void Debug_WriteString(const std::string& message)
	{
		OutputDebugStringA((message + "\n").c_str());
	}
#pragma endregion

#pragma region Vector Logging
	/// <summary>
	/// Logs a vector with a specified object and vector type.
	/// </summary>
	/// <param name="objectType">The type of the object.</param>
	/// <param name="vectorType">The type of the vector.</param>
	/// <param name="vector">The vector to log.</param>
	static void Debug_WriteVector(const std::string& objectType, const std::string& vectorType, const Vector& vector)
	{
		std::string message = objectType + " - " + vectorType + " - X: " + std::to_string(vector.x) + ", Y: " +
			std::to_string(vector.y) + ", Z: " + std::to_string(vector.z) + "\n";
		OutputDebugStringA(message.c_str());
	}

	/// <summary>
	/// Logs a DirectX vector with a specified object and vector type.
	/// </summary>
	/// <param name="objectType">The type of the object.</param>
	/// <param name="vectorType">The type of the vector.</param>
	/// <param name="vector">The vector to log.</param>
	static void Debug_WriteVector(const std::string& objectType, const std::string& vectorType,
		const DirectX::XMFLOAT3& vector)
	{
		std::string message = objectType + " - " + vectorType + " - X: " + std::to_string(vector.x) + ", Y: " +
			std::to_string(vector.y) + ", Z: " + std::to_string(vector.z) + "\n";
		OutputDebugStringA(message.c_str());
	}
#pragma endregion

#pragma region Number Logging
	/// <summary>
	/// Logs a number with a specified string.
	/// </summary>
	/// <param name="string">The string to log.</param>
	/// <param name="number">The number to log.</param>
	static void Debug_WriteNumber(const std::string& string, const float& number)
	{
		std::string message = string + std::to_string(number) + "\n";
		OutputDebugStringA(message.c_str());
	}
#pragma endregion

#pragma region Formatted Logging
	/// <summary>
	/// Logs a formatted string using a variable argument list.
	/// </summary>
	/// <param name="format">The format string.</param>
	/// <param name="args">The variable argument list.</param>
	/// <returns>The number of characters written.</returns>
	static int VDebugPrintF(const char* format, va_list args)
	{
		constexpr UINT32 MAX_CHARS = 1024;
		static char s_buffer[MAX_CHARS];

		int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, args);
		OutputDebugStringA(s_buffer);

		return charsWritten;
	}

	/// <summary>
	/// Logs a formatted string.
	/// </summary>
	/// <param name="format">The format string.</param>
	/// <returns>The number of characters written.</returns>
	static int DebugPrintF(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		int charsWritten = VDebugPrintF(format, argList);
		va_end(argList);

		return charsWritten;
	}
#pragma endregion
};