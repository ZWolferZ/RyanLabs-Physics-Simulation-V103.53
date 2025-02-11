#include "GameObject.h"

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle) : _type(type)
{
	_parent = nullptr;
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

// I was going to fold the maths at the start of these functions into a helper function,
// but my brain stopped working.

// I have been looking at this shit for like 8 hours,
// this is good enough.

void GameObject::HandleAABBABBB(const Vector& collisionNormal, float objectBMass, const Vector& objectBVelocity,
	const Vector& objectAMin, const Vector& objectAMax, const Vector& objectBMin,
	const Vector& objectBMax) const
{
	if (_physicsModel->GetMass() == 0) { return; }

	Vector relativeVelocity = _physicsModel->GetVelocity() - objectBVelocity;

	// The e constant breaks this for some reason
	float vj = Vector::Dot(relativeVelocity, collisionNormal);

	float inverseMassA = 1.0f / _physicsModel->GetMass();
	float inverseMassB = (objectBMass != 0.0f) ? 1.0f / objectBMass : 0.0f;

	float j = -(1.0f - _restitution) * vj / (inverseMassA + inverseMassB);

	Vector impulse = collisionNormal * j;

	float overlapX = min(objectAMax.x, objectBMax.x) - max(objectAMin.x, objectBMin.x);
	float overlapY = min(objectAMax.y, objectBMax.y) - max(objectAMin.y, objectBMin.y);
	float overlapZ = min(objectAMax.z, objectBMax.z) - max(objectAMin.z, objectBMin.z);

	Vector newPosition = Vector(overlapX * collisionNormal.x, overlapY * collisionNormal.y, overlapZ * collisionNormal.z);

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

	auto closestPoint = Vector(
		max(collisionData.ObjectBMinPoints.x, min(collisionData.SphereCenter.x, collisionData.ObjectBMaxPoints.x)),
		max(collisionData.ObjectBMinPoints.y, min(collisionData.SphereCenter.y, collisionData.ObjectBMaxPoints.y)),
		max(collisionData.ObjectBMinPoints.z, min(collisionData.SphereCenter.z, collisionData.ObjectBMaxPoints.z))
	);

	// Clean as fuck vector maths
	float distance = (collisionData.SphereCenter - closestPoint).Magnitude();

	float penetrationDepth = collisionData.SphereRadius - distance;

	Vector newPosition = collisionData.CollisionNormal * penetrationDepth;

	GetTransform()->SetPosition(GetTransform()->GetPosition() + newPosition);

	_physicsModel->ApplyImpulse(impulse);
}

void GameObject::HandleSphereSphere(const Vector& collisionNormal, float objectBMass, const Vector& objectBVelocity,
	float objectARadius, float objectBRadius, const Vector& objectAPosition,
	const Vector& objectBPosition) const
{
	if (_physicsModel->GetMass() == 0) { return; }

	Vector relativeVelocity = _physicsModel->GetVelocity() - objectBVelocity;

	float vj = Vector::Dot(relativeVelocity, collisionNormal);

	float inverseMassA = 1.0f / _physicsModel->GetMass();
	float inverseMassB = (objectBMass != 0.0f) ? 1.0f / objectBMass : 0.0f;

	float j = -(1.0f - _restitution) * vj / (inverseMassA + inverseMassB);

	Vector impulse = collisionNormal * j;

	float distance = (objectAPosition - objectBPosition).Magnitude();
	float penetrationDepth = (objectARadius + objectBRadius) - distance;

	Vector newPosition = collisionNormal * penetrationDepth;

	GetTransform()->SetPosition(GetTransform()->GetPosition() + newPosition);
	_physicsModel->ApplyImpulse(impulse);
}