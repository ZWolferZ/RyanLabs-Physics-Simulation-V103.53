#pragma once
#include "GameObject.h"
#include "RigidBodyModel.h"

class SphereCollider;
class AABB_Collider;

class Collider abstract
{
protected:
	Transform* _transform;

public:
	Collider(Transform* tranform) { _transform = tranform; }

	virtual bool CollidesWith(Collider& other) = 0;
	virtual bool CollidesWith(SphereCollider& other) = 0;
	virtual bool CollidesWith(AABB_Collider& other) = 0;
	virtual Vector GetCollisionNormal(const Collider& other) = 0;
	void HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB);
	virtual void Update() = 0;
	virtual std::string GetType() const = 0;

	Vector GetPosition() const { return _transform->GetPosition(); }
};
