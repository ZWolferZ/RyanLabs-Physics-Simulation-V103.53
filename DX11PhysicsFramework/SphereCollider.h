#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	float radius = 1.0f;

public:
	SphereCollider(Transform* transform, float r) : Collider(transform) { radius = r; }

	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	bool CollidesWith(SphereCollider& other) override;
	bool CollidesWith(AABBCollider& other) override;
	Vector GetCollisionNormal(const Collider& other) override;
	std::string GetType() const override { return "SphereCollider"; }

	void Update() override
	{
	}

	float GetRadius() const { return radius; }
};
