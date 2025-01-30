#include "Collider.h"
#include "SphereCollider.h"
#include "AABBCollider.h"

// I am only resolving the game object A collision because the game object B collision will be resolved in another pass.

void Collider::HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB)
{
	if (!gameObjectA->_collisionEnabled || !gameObjectB->_collisionEnabled) return;

	// Get the other object's collision normal
	Vector collisionNormal = this->GetCollisionNormal(*gameObjectB->GetPhysicsModel()->GetCollider());

	if ((gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" &&
		gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider") ||
		(gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" &&
			gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider")
		)
	{
		if ((gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider"))
		{
			auto sphere = dynamic_cast<const SphereCollider*>(gameObjectA->GetPhysicsModel()->GetCollider());
			auto aabb = dynamic_cast<const AABBCollider*>(gameObjectB->GetPhysicsModel()->GetCollider());

			gameObjectA->HandleSphereAABB(
				collisionNormal,
				gameObjectB->GetPhysicsModel()->GetMass(),
				gameObjectB->GetPhysicsModel()->GetVelocity(),
				sphere->GetPosition(),
				sphere->GetRadius(),
				aabb->GetMinPoints(),
				aabb->GetMaxPoints());
		}

		// I can't get the sphere to push the aabb,
		// I think its super easy but im so tired right now.
	}

	if (
		(gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" &&
			gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider") ||
		(gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" &&
			gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider")
		)
	{
		auto sphere1 = dynamic_cast<const SphereCollider*>(gameObjectA->GetPhysicsModel()->GetCollider());
		auto sphere2 = dynamic_cast<const SphereCollider*>(gameObjectB->GetPhysicsModel()->GetCollider());

		gameObjectA->HandleSphereSphere(
			collisionNormal,
			gameObjectB->GetPhysicsModel()->GetMass(),
			gameObjectB->GetPhysicsModel()->GetVelocity(),
			sphere1->GetRadius(),
			sphere2->GetRadius(),
			sphere1->GetPosition(),
			sphere2->GetPosition());
	}

	if (
		(gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider" &&
			gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider") ||
		(gameObjectB->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider" &&
			gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "AABB_Collider")
		)
	{
		auto aabb1 = dynamic_cast<const AABBCollider*>(gameObjectA->GetPhysicsModel()->GetCollider());
		auto aabb2 = dynamic_cast<const AABBCollider*>(gameObjectB->GetPhysicsModel()->GetCollider());

		gameObjectA->HandleAABBABBB(
			collisionNormal,
			gameObjectB->GetPhysicsModel()->GetMass(),
			gameObjectB->GetPhysicsModel()->GetVelocity(),
			aabb1->GetMinPoints(),
			aabb1->GetMaxPoints(),
			aabb2->GetMinPoints(),
			aabb2->GetMaxPoints()
		);
	}
}