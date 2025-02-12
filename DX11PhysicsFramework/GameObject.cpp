#pragma region Includes
// Include{s}
#include "GameObject.h"
#pragma endregion

#pragma region Constructors
GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle) : _type(type)
{
	_parent = nullptr;

	// Create the transform, appearance, and physics model
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(geometry, textureRV, material);
	_mass = mass;

	_transform->SetMass(_mass);

	if (isparticle)
	{
		_physicsModel = new ParticleModel(_transform, _mass);
	}
	else
	{
		_physicsModel = new RigidBodyModel(_transform, _mass);
	}
}

GameObject::GameObject(const string& type, const string& meshpath, const Material& material,
	ID3D11ShaderResourceView* textureRV, ID3D11Device& device, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle)
{
	_parent = nullptr;

	// Create the transform, appearance, and physics model
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(meshpath, device, textureRV, material);
	_mass = mass;
	_transform->SetMass(_mass);
	if (isparticle)
	{
		_physicsModel = new ParticleModel(_transform, _mass);
	}
	else
	{
		_physicsModel = new RigidBodyModel(_transform, _mass);
	}
}

GameObject::~GameObject()
{
	if (_transform != nullptr)
	{
		delete _transform;
		_transform = nullptr;
	}

	if (_appearance != nullptr)
	{
		delete _appearance;
		_appearance = nullptr;
	}

	if (_parent != nullptr)
	{
		delete _parent;
		_parent = nullptr;
	}

	if (_physicsModel != nullptr)
	{
		delete _physicsModel;
		_physicsModel = nullptr;
	}
}
#pragma endregion

#pragma region Update Method
void GameObject::Update(const float dt) const
{
	_physicsModel->Update(dt);

	_transform->Update(_physicsModel->GetIntegratedPosition(dt), dt);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldFloat4X4(),
			this->GetTransform()->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}
#pragma endregion

#pragma region Collision Methods
// I was going to fold the maths at the start of these functions into a helper function,
// but my brain stopped working.

// I have been looking at this shit for like 8 hours,
// this is good enough.

void GameObject::HandleAABBABBB(const AABBAABBCollisionManifold& collisionData) const
{
	if (_physicsModel->GetMass() == 0) { return; } // Return early so some weird maths don't happen.

	Vector relativeVelocity = _physicsModel->GetVelocity() - collisionData.ObjectBVelocity;

	// The e constant breaks this for some reason
	float vj = Vector::Dot(relativeVelocity, collisionData.CollisionNormal);

	// Inverse the masses
	float inverseMassA = 1.0f / _physicsModel->GetMass();
	float inverseMassB = (collisionData.ObjectBMass != 0.0f) ? 1.0f / collisionData.ObjectBMass : 0.0f;

	// Calculate the whatever J stands for (I forgot)
	float j = -(1.0f - _restitution) * vj / (inverseMassA + inverseMassB);

	// This makes sense
	Vector impulse = collisionData.CollisionNormal * j;

	// Calculate all the overlap between the two AABB's
	float overlapX = min(collisionData.ObjectAMaxPoints.x, collisionData.ObjectBMaxPoints.x) - max(collisionData.ObjectAMinPoints.x, collisionData.ObjectBMinPoints.x);
	float overlapY = min(collisionData.ObjectAMaxPoints.y, collisionData.ObjectBMaxPoints.y) - max(collisionData.ObjectAMinPoints.y, collisionData.ObjectBMinPoints.y);
	float overlapZ = min(collisionData.ObjectAMaxPoints.z, collisionData.ObjectBMaxPoints.z) - max(collisionData.ObjectAMinPoints.z, collisionData.ObjectBMinPoints.z);

	// Calculate the penetration depth
	Vector newPosition = Vector(overlapX * collisionData.CollisionNormal.x, overlapY * collisionData.CollisionNormal.y, overlapZ * collisionData.CollisionNormal.z);

	// Move the object out of the other object
	GetTransform()->SetPosition(GetTransform()->GetPosition() + newPosition);

	_physicsModel->ApplyImpulse(impulse);
}

void GameObject::HandleSphereAABB(const SPHEREAABBCollisionManifold& collisionData) const
{
	if (_physicsModel->GetMass() == 0) { return; }

	Vector relativeVelocity = _physicsModel->GetVelocity() - collisionData.ObjectBVelocity;

	float vj = Vector::Dot(relativeVelocity, collisionData.CollisionNormal);

	float inverseMassA = 1.0f / _physicsModel->GetMass();
	float inverseMassB = (collisionData.ObjectBMass != 0.0f) ? 1.0f / collisionData.ObjectBMass : 0.0f;

	float j = -(1.0f - _restitution) * vj / (inverseMassA + inverseMassB);

	Vector impulse = collisionData.CollisionNormal * j;

	// Similar to the AABB-AABB collision but use the sphere center instead of the min and max points
	Vector closestPoint = Vector(
		max(collisionData.ObjectBMinPoints.x, min(collisionData.SphereCenter.x, collisionData.ObjectBMaxPoints.x)),
		max(collisionData.ObjectBMinPoints.y, min(collisionData.SphereCenter.y, collisionData.ObjectBMaxPoints.y)),
		max(collisionData.ObjectBMinPoints.z, min(collisionData.SphereCenter.z, collisionData.ObjectBMaxPoints.z))
	);

	// Clean as fuck vector maths
	float distance = (collisionData.SphereCenter - closestPoint).Magnitude();

	// Work out the penetration depth
	float penetrationDepth = collisionData.SphereRadius - distance;

	Vector newPosition = collisionData.CollisionNormal * penetrationDepth;

	// Move the objects outside each other
	GetTransform()->SetPosition(GetTransform()->GetPosition() + newPosition);

	_physicsModel->ApplyImpulse(impulse);
}

void GameObject::HandleSphereSphere(const SPHERESPHERECollisionManifold& collisionData) const
{
	if (_physicsModel->GetMass() == 0) { return; }

	Vector relativeVelocity = _physicsModel->GetVelocity() - collisionData.ObjectBVelocity;

	float vj = Vector::Dot(relativeVelocity, collisionData.CollisionNormal);

	float inverseMassA = 1.0f / _physicsModel->GetMass();
	float inverseMassB = (collisionData.ObjectBMass != 0.0f) ? 1.0f / collisionData.ObjectBMass : 0.0f;

	float j = -(1.0f - _restitution) * vj / (inverseMassA + inverseMassB);

	Vector impulse = collisionData.CollisionNormal * j;

	// This is the easiest one to calculate the penetration depth for.
	float distance = (collisionData.SphereACenter - collisionData.SphereBCenter).Magnitude();

	// Calculate the penetration depth
	float penetrationDepth = (collisionData.SphereARadius + collisionData.SphereBRadius) - distance;

	Vector newPosition = collisionData.CollisionNormal * penetrationDepth;

	// Move the objects outside each other
	GetTransform()->SetPosition(GetTransform()->GetPosition() + newPosition);

	_physicsModel->ApplyImpulse(impulse);
}
#pragma endregion