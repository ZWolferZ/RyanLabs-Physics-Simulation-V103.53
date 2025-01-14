#include "Collider.h"

void Collider::HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB)
{
	if (!gameObjectA->_collisionEnabled || !gameObjectB->_collisionEnabled) return;

	// Get the other object's collision normal
	Vector collisionNormal = this->GetCollisionNormal(*gameObjectB->GetPhysicsModel()->GetCollider());

	// Adjust velocity to stop movement in the direction of collision
	Vector velocity = gameObjectA->GetPhysicsModel()->GetVelocity();
	Vector newVelocity = velocity - collisionNormal * Vector::Dot(velocity, collisionNormal);
	gameObjectA->GetPhysicsModel()->SetVelocity(newVelocity);
}