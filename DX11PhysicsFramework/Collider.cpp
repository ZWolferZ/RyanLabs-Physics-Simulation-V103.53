#include "Collider.h"

void Collider::HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB)
{
	// Get the other objects collision normal
	Vector collisionNormal = this->GetCollisionNormal(*gameObjectB->GetPhysicsModel()->GetCollider());

	// Adjust velocity to stop movement in the direction of collision
	gameObjectA->GetPhysicsModel()->SetVelocity(
		gameObjectA->GetPhysicsModel()->GetVelocity() - collisionNormal * Vector::Dot(gameObjectA->GetPhysicsModel()->GetVelocity(), collisionNormal));
}