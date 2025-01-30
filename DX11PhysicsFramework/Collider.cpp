#include "Collider.h"

void Collider::HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB)
{
	if (!gameObjectA->_collisionEnabled || !gameObjectB->_collisionEnabled) return;

	// Get the other object's collision normal
	Vector collisionNormal = this->GetCollisionNormal(*gameObjectB->GetPhysicsModel()->GetCollider());
	Vector velocity = gameObjectB->GetPhysicsModel()->GetVelocity();
	float mass = gameObjectB->GetPhysicsModel()->GetMass();
	bool SPHEREandAABB = false;
	bool SPHEREandSPHERE = false;

	NormalCollided collided = None;

	if (collisionNormal == Vector(0, 1.0f, 0))
	{
		// Top collision
		collided = Top;
	}
	else if (collisionNormal == Vector(0, -1.0f, 0))
	{
		// Bottom collision
		collided = Bottom;
	}
	else if (collisionNormal == Vector(1.0f, 0, 0))
	{
		// Right collision
		collided = Right;
	}
	else if (collisionNormal == Vector(-1.0f, 0, 0))
	{
		// Left collision
		collided = Left;
	}
	else if (collisionNormal == Vector(0, 0, 1.0f))
	{
		// Front collision
		collided = Front;
	}
	else if (collisionNormal == Vector(0, 0, -1.0f))
	{
		// Back collision
		collided = Back;
	}

	if (gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" && gameObjectB->GetPhysicsModel()->
		GetCollider()->GetType() == "AABB_Collider")
	{
		SPHEREandAABB = true;
	}
	if (gameObjectA->GetPhysicsModel()->GetCollider()->GetType() == "SphereCollider" && gameObjectB->GetPhysicsModel()->
		GetCollider()->GetType() == "SphereCollider")
	{
		SPHEREandSPHERE = true;
	}

	gameObjectA->WallCollided(collided, collisionNormal, mass, velocity, SPHEREandAABB, SPHEREandSPHERE);
}