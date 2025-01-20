#pragma once
#include "Collider.h"

class AABB_Collider : public Collider

{
	Vector _halfExtents;
	Vector _minPoints;
	Vector _maxPoints;
	Vector _minExtent;
	float dx, dy, dz;

public:
	AABB_Collider(Transform* transform, Vector minPoints, Vector maxPoints) : Collider(transform)
	{
		_minPoints = minPoints;
		_maxPoints = maxPoints;
		_minExtent = _minPoints - _transform->GetPosition();
		dx = _maxPoints.x - _minPoints.x;
		dy = _maxPoints.y - _minPoints.y;
		dz = _maxPoints.z - _minPoints.z;

		_halfExtents = Vector(dx / 2, dy / 2, dz / 2);
	}

	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	bool CollidesWith(SphereCollider& other) override;
	bool CollidesWith(AABB_Collider& other) override;
	Vector GetCollisionNormal(const Collider& other) override;
	Vector GetMinPoints() const { return _minPoints; }
	Vector GetMaxPoints() const { return _maxPoints; }
	Vector GetHalfExtents() const { return _halfExtents; }
	std::string GetType() const override { return "AABB_Collider"; }
	void Update() override;
};
