#pragma once
#include "Collider.h"

class AABB_Collider : public Collider

{
	// We just don't need half extents here,
	// I kinda winged it, and it works for some reason.

	// I think these are a variation of AABB colliders, which I have not seen anyone do before.
	// So I am calling them RAABB colliders, or RYAN-AXIS-ALIGNED-BOUNDING-BOXES if you have the time.

	// It is a pattern pending collider construction, WITH TEN TIMES MORE EFFICIENCY (Or something like that).

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
	}

	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	bool CollidesWith(SphereCollider& other) override;
	bool CollidesWith(AABB_Collider& other) override;
	Vector GetCollisionNormal(const Collider& other) override;
	Vector GetMinPoints() const { return _minPoints; }
	Vector GetMaxPoints() const { return _maxPoints; }
	void Update() override;
};
