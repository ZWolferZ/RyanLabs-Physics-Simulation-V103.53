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

			SPHEREAABBCollisionManifold collisionData =
			{
			collisionData.CollisionNormal = collisionNormal,
			collisionData.ObjectBMass = gameObjectB->GetPhysicsModel()->GetMass(),
			collisionData.ObjectBVelocity = gameObjectB->GetPhysicsModel()->GetVelocity(),
			collisionData.SphereCenter = sphere->GetPosition(),
			collisionData.SphereRadius = sphere->GetRadius(),
			collisionData.ObjectBMinPoints = aabb->GetMinPoints(),
			collisionData.ObjectBMaxPoints = aabb->GetMaxPoints()
			};

			gameObjectA->HandleSphereAABB(collisionData);
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

		SPHERESPHERECollisionManifold collisionData =
		{
			collisionData.CollisionNormal = collisionNormal,
			collisionData.ObjectBMass = gameObjectB->GetPhysicsModel()->GetMass(),
			collisionData.ObjectBVelocity = gameObjectB->GetPhysicsModel()->GetVelocity(),
			collisionData.SphereARadius = sphere1->GetRadius(),
			collisionData.SphereBRadius = sphere2->GetRadius(),
			collisionData.SphereACenter = sphere1->GetPosition(),
			collisionData.SphereBCenter = sphere2->GetPosition()
		};

		gameObjectA->HandleSphereSphere(collisionData);
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

		AABBAABBCollisionManifold collisionData =
		{
			collisionData.CollisionNormal = collisionNormal,
			collisionData.ObjectBMass = gameObjectB->GetPhysicsModel()->GetMass(),
			collisionData.ObjectBVelocity = gameObjectB->GetPhysicsModel()->GetVelocity(),
			collisionData.ObjectAMinPoints = aabb1->GetMinPoints(),
			collisionData.ObjectAMaxPoints = aabb1->GetMaxPoints(),
			collisionData.ObjectBMinPoints = aabb2->GetMinPoints(),
			collisionData.ObjectBMaxPoints = aabb2->GetMaxPoints()
		};

		gameObjectA->HandleAABBABBB(collisionData);
	}
}