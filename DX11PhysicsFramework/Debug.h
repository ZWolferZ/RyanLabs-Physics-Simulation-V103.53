#pragma once
#include <windows.h>
#include <string>

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
};
