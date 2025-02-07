#pragma once
#pragma region Includes
// Include{s}
#include "Collider.h"
#pragma endregion

/// <summary>
/// Stores and calculates data for axis aligned bounding boxes
/// </summary>
class AABBCollider : public Collider

{
private:
#pragma region Member Variables
	/// <summary>
	/// Member variable data
	/// </summary>
	Vector _halfExtents;
	Vector _minPoints;
	Vector _maxPoints;
	Vector _minExtent;
	float dx, dy, dz;
#pragma endregion

public:
	/// <summary>
	/// Creates the AABB from the transform of the object as the center and the minium and maxium points specifed.
	/// </summary>
	/// <param name="transform">Transform of the Gameobject</param>
	/// <param name="minPoints">Minimum Points of the bounding box</param>
	/// <param name="maxPoints">Maximum Points of the bounding box</param>
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

#pragma region Collider Check Functions
	/// <summary>
	/// General base class overider
	/// </summary>
	/// <param name="other">other collider</param>
	/// /// <returns>True if collision, False if not</returns>
	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }

	/// <summary>
	/// AABB / Sphere Collider Test
	/// </summary>
	/// <param name="other">other sphere collider</param>
	/// <returns>True if collision, False if not</returns>
	bool CollidesWith(SphereCollider& other) override;

	/// <summary>
	/// AABB / AABB
	/// </summary>
	/// <param name="other">other AABB Collider</param>
	/// <returns>True if collision, False if not</returns>
	bool CollidesWith(AABBCollider& other) override;
#pragma endregion

#pragma  region GettersandSetters
	/// <summary>
	/// Generate the collision normal between AABB's
	/// </summary>
	/// <param name="other">other collider</param>
	/// <returns>The collision normal</returns>
	Vector GetCollisionNormal(const Collider& other) override;

	/// <summary>
	/// Return the minmium points of the AABB
	/// </summary>
	Vector GetMinPoints() const { return _minPoints; }

	/// <summary>
	/// Return the maximum points of the AABB
	/// </summary>
	Vector GetMaxPoints() const { return _maxPoints; }

	/// <summary>
	/// Return the half extents of the AABB
	/// </summary>
	Vector GetHalfExtents() const { return _halfExtents; }

	/// <summary>
	/// Returns the type of the collider as a string
	/// </summary>
	/// <returns>String of the name of the collider</returns>
	std::string GetType() const override { return "AABB_Collider"; }

#pragma endregion

	/// <summary>
	/// Updates the position of the AABB collider
	/// </summary>
	void Update() override;
};
