#include "GameObject.h"

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(geometry, textureRV, material);

	if (isparticle)
	{
		_physicsModel = new ParticleModel(_transform, mass);
	}
	else
	{
		_physicsModel = new RigidBodyModel(_transform, mass);
	}
}

GameObject::GameObject(const string& type, const string& meshpath, const Material& material,
	ID3D11ShaderResourceView* textureRV, ID3D11Device& device, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle)
{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(meshpath, device, textureRV, material);

	if (isparticle)
	{
		_physicsModel = new ParticleModel(_transform, mass);
	}
	else
	{
		_physicsModel = new RigidBodyModel(_transform, mass);
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

	_transform->Update();

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldFloat4X4(),
			this->GetTransform()->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}

void GameObject::WallCollided(NormalCollided collided) const
{
	Vector currentVelocity = _physicsModel->GetVelocity();

	switch (collided)
	{
	case Top:
		if (currentVelocity.y < 0)
			currentVelocity.y = 0;

		_physicsModel->_simulateGravity = false;
		break;

	case Bottom:
		if (currentVelocity.y > 0)
			currentVelocity.y = 0;

		break;

	case Left:
		if (currentVelocity.x > 0)
			currentVelocity.x = 0;
		break;

	case Right:
		if (currentVelocity.x < 0)
			currentVelocity.x = 0;
		break;

	case Front:
		if (currentVelocity.z < 0)
			currentVelocity.z = 0;
		break;

	case Back:
		if (currentVelocity.z > 0)
			currentVelocity.z = 0;
		break;

	case None:
		Debug::Debug_WriteString(GetType() + " : NONE");
		break;

	default:
		break;
	}

	_physicsModel->SetVelocity(currentVelocity);
}