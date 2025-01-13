#pragma once
#include "RigidBodyModel.h"

class SphereCollider;

class Collider abstract
{
protected:
	Transform* _transform;
public:
	Collider(Transform* tranform) { _transform = tranform; }

	virtual bool CollidesWith(Collider& other) = 0;
	virtual bool CollidesWith(SphereCollider& other) = 0;

	Vector GetPosition() const { return _transform->GetPosition(); }
};

