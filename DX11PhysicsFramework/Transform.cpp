#include "Transform.h"

Transform::Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType)
{
	_position = position;
	SetRotation(rotation);
	_scale = scale;

	_originPosition = position;
	_originRotation = rotation;
	_originScale = scale;

	_objectType = objectType;
}

void Transform::Update(const Vector& integratedPosition, float deltaTime)
{
	_previousPosition = _position;
	_previousWorld = _world;

	_orientation += _orientation * MakeQFromEulerAngles(_angularVelocity.x, _angularVelocity.y, _angularVelocity.z) * 0.5f * deltaTime;

	_angularVelocity *= pow(_angularDamping, deltaTime);

	if (_orientation.Magnitude() != 1.0f)
	{
		_orientation /= _orientation.Magnitude();
	}

	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationQuaternion(GetRotationQuaternion());
	XMMATRIX translation = XMMatrixTranslation(integratedPosition.x, integratedPosition.y, integratedPosition.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
}

void Transform::Move(const Vector& direction, float deltaTime, float moveSpeed)
{
	_position += (Vector::Normalise(direction) * moveSpeed) * deltaTime;
	Debug::Debug_WriteVector(_objectType, "Position", _position);
}

void Transform::Rotate(const Vector& rotation, float deltaTime, float moveSpeed)
{
	Quaternion preRotation = MakeQFromEulerAngles(rotation.x * moveSpeed * deltaTime,
		rotation.y * moveSpeed * deltaTime,
		rotation.z * moveSpeed * deltaTime);

	Quaternion postRotate = GetOrientation() * preRotation;

	if (postRotate.Magnitude() != 1.0f)
	{
		postRotate /= postRotate.Magnitude();
	}

	SetOrientation(postRotate);
}

void Transform::Scale(const Vector& scale, float deltaTime, float moveSpeed)
{
	_scale += (Vector::Normalise(scale) * moveSpeed) * deltaTime;
	Debug::Debug_WriteVector(_objectType, "Scale", _scale);
}

void Transform::Reset()
{
	_position = _originPosition;
	SetRotation(_originRotation);
	_scale = _originScale;

	Debug::Debug_WriteString(_objectType + " Reset");
}