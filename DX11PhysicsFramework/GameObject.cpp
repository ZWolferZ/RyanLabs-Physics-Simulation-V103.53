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

	_transform->Update(_physicsModel->GetIntegratedPosition(dt), dt);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldFloat4X4(),
			this->GetTransform()->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}

void GameObject::WallCollided(NormalCollided collided, const Vector& collisionNormal, float objectBmass,
	const Vector& objectBVelocity, bool SPHEREandAABB, bool SPHEREandSPHERE) const
{
	Vector currentVelocity = _physicsModel->GetVelocity();
	Vector relativeVelocity = currentVelocity - objectBVelocity;

	float normalVelocity = Vector::Dot(relativeVelocity, collisionNormal);

	// What the hell is denominator?
	float denominator = (1.0f / _physicsModel->GetMass()) + (1.0f / objectBmass);

	float impulseMagnitude = (-(1.0f + _restitution) * normalVelocity) / denominator;

	Vector impulse = collisionNormal * impulseMagnitude;

	_physicsModel->ApplyImpulse(impulse);

	switch (collided)
	{
	case Top:
		if (currentVelocity.y < 0)
		{
			currentVelocity.y = 0;

			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y + _positionCorrection,
				this->GetTransform()->GetPosition().z);
		}

		break;

	case Bottom:
		if (currentVelocity.y > 0)
		{
			currentVelocity.y = 0;

			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y - _positionCorrection,
				this->GetTransform()->GetPosition().z);
		}

		break;

	case Left:

		if (currentVelocity.x > _velocityClamp)
		{
			currentVelocity.x = _velocityClamp;
		}

		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x - _positionCorrection,
			this->GetTransform()->GetPosition().y, this->GetTransform()->GetPosition().z);

		if (SPHEREandAABB)
		{
			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x - _spherePositionCorrection,
				this->GetTransform()->GetPosition().y,
				this->GetTransform()->GetPosition().z);
		}

		break;

	case Right:
		if (currentVelocity.x < -_velocityClamp)
		{
			currentVelocity.x = -_velocityClamp;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x + _positionCorrection,
			this->GetTransform()->GetPosition().y, this->GetTransform()->GetPosition().z);

		if (SPHEREandAABB)
		{
			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x + _spherePositionCorrection,
				this->GetTransform()->GetPosition().y,
				this->GetTransform()->GetPosition().z);
		}
		break;

	case Front:
		if (currentVelocity.z < -_velocityClamp)
		{
			currentVelocity.z = -_velocityClamp;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x, this->GetTransform()->GetPosition().y,
			this->GetTransform()->GetPosition().z + _positionCorrection);

		if (SPHEREandAABB)
		{
			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y,
				this->GetTransform()->GetPosition().z + _spherePositionCorrection);
		}

		break;

	case Back:
		if (currentVelocity.z > _velocityClamp)
		{
			currentVelocity.z = _velocityClamp;
		}
		this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x, this->GetTransform()->GetPosition().y,
			this->GetTransform()->GetPosition().z - _positionCorrection);

		if (SPHEREandAABB)
		{
			this->GetTransform()->SetPosition(this->GetTransform()->GetPosition().x,
				this->GetTransform()->GetPosition().y,
				this->GetTransform()->GetPosition().z - _spherePositionCorrection);
		}

		break;

	case None:
		Debug::Debug_WriteString(GetType() + " : NONE");
		break;

	default:
		break;
	}

	if (SPHEREandSPHERE)
	{
		if (currentVelocity.y < 0 || currentVelocity.y > 0)
		{
			currentVelocity.y = 0;
		}
		if (currentVelocity.x < 0 || currentVelocity.x > 0)
		{
			currentVelocity.x = 0;
		}
		if (currentVelocity.z < 0 || currentVelocity.z > 0)
		{
			currentVelocity.z = 0;
		}
	}

	_physicsModel->SetVelocity(currentVelocity);
}