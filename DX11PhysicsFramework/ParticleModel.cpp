#include "ParticleModel.h"

void ParticleModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	_mass = _transform->GetMass();

	AddForce(force);
}