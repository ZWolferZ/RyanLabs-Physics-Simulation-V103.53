#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
public:
	XMFLOAT3X3 _inertiaTensor;

	Vector _angularVelocity = { 0, 0, 0 };

	bool _tensorSet = false;

	explicit RigidBodyModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;

		// This is funny to me
		_inertiaTensor = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}

	void AddRelativeForce(const Vector& force, const Vector& point, float deltaTime) override;

	void SetInertiaTensor();

	Vector CalculateAngularVelocity(Vector torque, float deltaTime) const;
};
