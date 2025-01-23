#include "GameObject.h"

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation, float mass, bool isparticle) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(geometry, textureRV, material);
	_mass = mass;

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

	_transform->Update(_physicsModel->GetIntegratedPosition(dt));

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldFloat4X4(),
			this->GetTransform()->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}

void GameObject::WallCollided(NormalCollided collided, const Vector& collisionNormal, float objectBmass,
	const Vector& objectBVelocity) const
{
	Vector currentVelocity = _physicsModel->GetVelocity();

	Vector relativeVelocity = currentVelocity - objectBVelocity;

	float vj = collisionNormal.Magnitude() * relativeVelocity.Magnitude() * (-(1.0f + 2.71828f));

	float J = vj * (-GetPhysicsModel()->GetMass() + -objectBmass);

	switch (collided)
	{
	case Top:
		if (currentVelocity.y < 0)
		{
			currentVelocity.y = 0;

			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y + 0.004f,
				this->GetTransform()->GetPosition().z);
		}

		break;

	case Bottom:
		if (currentVelocity.y > 0)
		{
			currentVelocity.y = 0;

			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y - 0.004f,
				this->GetTransform()->GetPosition().z);
		}

		break;

	case Left:
		if (currentVelocity.x > 0.3f)
		{
			currentVelocity.x = 0.3f;
		}

		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x - 0.004f,
			this->GetTransform()->GetPosition().y, this->GetTransform()->GetPosition().z);

		break;

	case Right:
		if (currentVelocity.x > 0.3f)
		{
			currentVelocity.x = 0.3f;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x + 0.004f,
			this->GetTransform()->GetPosition().y, this->GetTransform()->GetPosition().z);

		break;

	case Front:
		if (currentVelocity.z > 0.3f)
		{
			currentVelocity.z = 0.3f;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x, this->GetTransform()->GetPosition().y,
			this->GetTransform()->GetPosition().z + 0.004f);

		break;

	case Back:
		if (currentVelocity.z > 0.3f)
		{
			currentVelocity.z = 0.3f;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x, this->GetTransform()->GetPosition().y,
			this->GetTransform()->GetPosition().z - 0.004f);

		break;

	case None:
		Debug::Debug_WriteString(GetType() + " : NONE");
		break;

	default:
		break;
	}
	GetPhysicsModel()->ApplyImpulse(collisionNormal * (-GetPhysicsModel()->GetMass() * J));
	_physicsModel->SetVelocity(currentVelocity);
}