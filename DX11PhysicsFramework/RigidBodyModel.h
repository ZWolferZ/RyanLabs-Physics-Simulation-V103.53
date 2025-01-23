#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
public:
	XMFLOAT3X3 _inertiaTensor;
	float _angularDamping = 0.99f;

	explicit RigidBodyModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;
	

	}

	void AddRelativeForce(const Vector& force, const Vector& point) override;

	void SetInertiaTensor();
	
	
};
