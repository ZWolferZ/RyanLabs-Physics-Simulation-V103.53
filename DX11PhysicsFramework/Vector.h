#pragma once
#include <cmath>

constexpr float tol = 0.0001f;

class Vector
{
public:
	float x, y, z;

	// Constructor
	Vector(const float initX, const float initY, const float initZ)
	{
		x = initX;
		y = initY;
		z = initZ;
	}

	// Default Constructor
	Vector()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	// Add Operator Overload
	Vector operator+(const Vector b) const
	{
		return Vector{this->x + b.x, this->y + b.y, this->z + b.z};
	}

	Vector operator+=(const Vector b)
	{
		return Vector{this->x += b.x, this->y += b.y, this->z += b.z};
	}

	// Minus Operator Overload
	Vector operator-(const Vector b) const
	{
		return Vector{this->x - b.x, this->y - b.y, this->z - b.z};
	}

	Vector operator-=(const Vector b)
	{
		return Vector{this->x -= b.x, this->y -= b.y, this->z -= b.z};
	}

	// Multiply Operator Overload
	Vector operator*(const float scalar) const
	{
		return Vector{this->x * scalar, this->y * scalar, this->z * scalar};
	}

	Vector operator*(const int scalar) const
	{
		return Vector{this->x * scalar, this->y * scalar, this->z * scalar};
	}

	Vector operator*(const Vector b) const
	{
		return Vector{this->x * b.x, this->y * b.y, this->z * b.z};
	}

	Vector operator*=(const float scalar) const
	{
		return Vector{this->x * scalar, this->y * scalar, this->z * scalar};
	}

	Vector operator*=(const int scalar) const
	{
		return Vector{this->x * scalar, this->y * scalar, this->z * scalar};
	}

	Vector operator*=(const Vector b) const
	{
		return Vector{this->x * b.x, this->y * b.y, this->z * b.z};
	}

	// Divide Operator Overload
	Vector operator/(const float scalar) const
	{
		return Vector{this->x / scalar, this->y / scalar, this->z / scalar};
	}

	Vector operator/(const int scalar) const
	{
		return Vector{this->x / scalar, this->y / scalar, this->z / scalar};
	}

	Vector operator/(const Vector b) const
	{
		return Vector{this->x / b.x, this->y / b.y, this->z / b.z};
	}

	Vector operator/=(const float scalar) const
	{
		return Vector{this->x / scalar, this->y / scalar, this->z / scalar};
	}

	Vector operator/=(const int scalar) const
	{
		return Vector{this->x / scalar, this->y / scalar, this->z / scalar};
	}

	// Copy Operator Overload
	Vector operator=(const Vector b)
	{
		return Vector{this->x = b.x, this->y = b.y, this->z = b.z};
	}

	// Comparison Operator Overload
	bool operator==(const Vector b) const
	{
		return this->x == b.x && this->y == b.y && this->z == b.z;
	}

	bool operator!=(const Vector b) const
	{
		return this->x != b.x || this->y != b.y || this->z != b.z;
	}

	void Reverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	static Vector Reverse(const Vector vector)
	{
		return {-vector.x, -vector.y, -vector.z};
	}

	Vector Zero(Vector vector)
	{
		return {x = 0, y = 0, z = 0};
	}

	void Zero()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	/// <summary>
	///     Returns the magnitude of a vector
	/// </summary>
	static float Magnitude(const Vector vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	float Magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	/// <summary>
	///     Returns the normalised vector of a vector
	/// </summary>
	static Vector Normalise(const Vector a)
	{
		float mag = Magnitude(a);

		return
			mag > tol
				? a / mag
				: Vector(0, 0, 0); // Super cool ternary expression to check if magnitude is greater than 0
	}

	Vector Normalise() const
	{
		const auto thisVector = Vector{x, y, z};
		const float mag = Magnitude(thisVector);

		return
			mag > tol
				? thisVector / mag
				: Vector(0, 0, 0); // Super cool ternary expression to check if magnitude is greater than 0
	}

	/// <summary>
	///     Returns the dot product of two vectors
	/// </summary>
	static float Dot(const Vector a, const Vector b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/// <summary>
	   ///     Returns the cross product of two vectors
	   /// </summary>
	static Vector Cross(const Vector a, const Vector b)
	{
		return Vector{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
};
