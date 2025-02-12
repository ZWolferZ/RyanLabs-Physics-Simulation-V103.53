#pragma once

#pragma region Includes
// Include{s}
#include "Structures.h"
#pragma endregion

/// <summary>
/// Represents the transform of an object, including position, rotation, and scale.
/// </summary>
class Transform
{
public:
#pragma region Constructor
	/// <summary>
	/// Initializes a new instance of the Transform class.
	/// </summary>
	/// <param name="position">The position of the object.</param>
	/// <param name="rotation">The rotation of the object.</param>
	/// <param name="scale">The scale of the object.</param>
	/// <param name="objectType">The type of the object.</param>
	Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType);
#pragma endregion

#pragma region Update Method
	/// <summary>
	/// Updates the transform with the integrated position and delta time.
	/// </summary>
	/// <param name="integratedPosition">The integrated position.</param>
	/// <param name="deltaTime">The time between the last frame.</param>
	void Update(const Vector& integratedPosition, float deltaTime);
#pragma endregion

#pragma region Getters
	/// <summary>
	/// Gets the position of the object.
	/// </summary>
	/// <returns>The position of the object.</returns>
	Vector GetPosition() const { return _position; }

	/// <summary>
	/// Gets the scale of the object.
	/// </summary>
	/// <returns>The scale of the object.</returns>
	Vector GetScale() const { return _scale; }

	/// <summary>
	/// Gets the rotation of the object.
	/// </summary>
	/// <returns>The rotation of the object.</returns>
	Vector GetRotation() { return MakeEulerAnglesFromQ(_orientation); }

	/// <summary>
	/// Gets the rotation quaternion of the object.
	/// </summary>
	/// <returns>The rotation quaternion of the object.</returns>
	XMVECTOR GetRotationQuaternion()
	{
		return XMVectorSet(_orientation.GetVector().x, _orientation.GetVector().y, _orientation.GetVector().z,
			_orientation.GetScalar());
	}

	/// <summary>
	/// Gets the orientation of the object.
	/// </summary>
	/// <returns>The orientation of the object.</returns>
	Quaternion GetOrientation() { return _orientation; }

	/// <summary>
	/// Gets the angular velocity of the object.
	/// </summary>
	/// <returns>The angular velocity of the object.</returns>
	Vector GetAngularVelocity() { return _angularVelocity; }

	/// <summary>
	/// Gets the world matrix of the object.
	/// </summary>
	/// <returns>The world matrix of the object.</returns>
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	/// <summary>
	/// Gets the previous world matrix of the object.
	/// </summary>
	/// <returns>The previous world matrix of the object.</returns>
	XMMATRIX GetPreviousWorldMatrix() const { return XMLoadFloat4x4(&_previousWorld); }

	/// <summary>
	/// Gets the world matrix of the object as a float4x4.
	/// </summary>
	/// <returns>The world matrix of the object as a float4x4.</returns>
	XMFLOAT4X4* GetWorldFloat4X4() { return &_world; }

	/// <summary>
	/// Gets the previous position of the object.
	/// </summary>
	/// <returns>The previous position of the object.</returns>
	Vector GetPreviousPosition() const { return _previousPosition; }

	/// <summary>
	/// Gets the mass of the object.
	/// </summary>
	/// <returns>The mass of the object.</returns>
	float GetMass() { return _mass; }
#pragma endregion

#pragma region Setters
	/// <summary>
	/// Sets the position of the object.
	/// </summary>
	/// <param name="newPosition">The new position of the object.</param>
	void SetPosition(const Vector& newPosition) { _position = newPosition; }

	/// <summary>
	/// Sets the position of the object.
	/// </summary>
	/// <param name="x">The x-coordinate of the new position.</param>
	/// <param name="y">The y-coordinate of the new position.</param>
	/// <param name="z">The z-coordinate of the new position.</param>
	void SetPosition(const float x, const float y, const float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	/// <summary>
	/// Sets the scale of the object.
	/// </summary>
	/// <param name="newScale">The new scale of the object.</param>
	void SetScale(const Vector& newScale) { _scale = newScale; }

	/// <summary>
	/// Sets the scale of the object.
	/// </summary>
	/// <param name="x">The x-coordinate of the new scale.</param>
	/// <param name="y">The y-coordinate of the new scale.</param>
	/// <param name="z">The z-coordinate of the new scale.</param>
	void SetScale(const float x, const float y, const float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	/// <summary>
	/// Sets the rotation of the object.
	/// </summary>
	/// <param name="newRotation">The new rotation of the object.</param>
	void SetRotation(const Vector& newRotation)
	{
		_orientation = MakeQFromEulerAngles(newRotation.x, newRotation.y, newRotation.z);
	}

	/// <summary>
	/// Sets the rotation of the object.
	/// </summary>
	/// <param name="x">The x-coordinate of the new rotation.</param>
	/// <param name="y">The y-coordinate of the new rotation.</param>
	/// <param name="z">The z-coordinate of the new rotation.</param>
	void SetRotation(const float x, const float y, const float z) { _orientation = MakeQFromEulerAngles(x, y, z); }

	/// <summary>
	/// Sets the orientation of the object.
	/// </summary>
	/// <param name="orientation">The new orientation of the object.</param>
	void SetOrientation(Quaternion orientation) { _orientation = orientation; }

	/// <summary>
	/// Sets the angular velocity of the object.
	/// </summary>
	/// <param name="velocity">The new angular velocity of the object.</param>
	/// <returns>The updated angular velocity.</returns>
	Vector SetAngularVelocity(Vector velocity) { return _angularVelocity = velocity; }

	/// <summary>
	/// Sets the mass of the object.
	/// </summary>
	/// <param name="newMass">The new mass of the object.</param>
	/// <returns>The updated mass.</returns>
	float SetMass(float newMass) { return _mass = newMass; }
#pragma endregion

#pragma region Movement Methods
	/// <summary>
	/// Moves the object in a specified direction.
	/// </summary>
	/// <param name="direction">The direction to move.</param>
	/// <param name="deltaTime">The delta time.</param>
	/// <param name="moveSpeed">The speed of movement.</param>
	void Move(const Vector& direction, float deltaTime, float moveSpeed);

	/// <summary>
	/// Rotates the object by a specified amount.
	/// </summary>
	/// <param name="rotation">The amount to rotate.</param>
	/// <param name="deltaTime">The delta time.</param>
	/// <param name="moveSpeed">The speed of rotation.</param>
	void Rotate(const Vector& rotation, float deltaTime, float moveSpeed);

	/// <summary>
	/// Scales the object by a specified amount.
	/// </summary>
	/// <param name="scale">The amount to scale.</param>
	/// <param name="deltaTime">The delta time.</param>
	/// <param name="moveSpeed">The speed of scaling.</param>
	void Scale(const Vector& scale, float deltaTime, float moveSpeed);
#pragma endregion

#pragma region Reset Method
	/// <summary>
	/// Resets the transform to its original state.
	/// </summary>
	void Reset();
#pragma endregion

private:
#pragma region Member Variables
	Vector _position;
	Vector _previousPosition;
	Vector _scale;
	Quaternion _orientation;
	Vector _angularVelocity;
	float _angularDamping = 0.80f;
	float _depth = 0.0f;
	float _mass = 0.0f;

	Vector _originPosition;
	Vector _originScale;
	Vector _originRotation;

	std::string _objectType;

	XMFLOAT4X4 _world;
	XMFLOAT4X4 _previousWorld;
#pragma endregion
};