#pragma once
#include "Collider.h"

class AABBCollider : public Collider

{
	Vector _halfExtents;
	Vector _minPoints;
	Vector _maxPoints;
	Vector _minExtent;
	float dx, dy, dz;

public:
	AABBCollider(Transform* transform, Vector minPoints, Vector maxPoints) : Collider(transform)
	{
		_minPoints = minPoints;
		_maxPoints = maxPoints;
		_minExtent = _minPoints - _transform->GetPosition();
		dx = _maxPoints.x - _minPoints.x;
		dy = _maxPoints.y - _minPoints.y;
		dz = _maxPoints.z - _minPoints.z;

		_halfExtents = Vector(dx / 2.0f, dy / 2.0f, dz / 2.0f);
	}

	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	bool CollidesWith(SphereCollider& other) override;
	bool CollidesWith(AABBCollider& other) override;
	Vector GetCollisionNormal(const Collider& other) override;
	Vector GetMinPoints() const { return _minPoints; }
	Vector GetMaxPoints() const { return _maxPoints; }
	Vector GetHalfExtents() const { return _halfExtents; }
	std::string GetType() const override { return "AABB_Collider"; }
	void Update() override;
};
