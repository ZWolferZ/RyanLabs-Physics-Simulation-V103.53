#include "ParticleModel.h"

void ParticleModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	AddForce(force);
}