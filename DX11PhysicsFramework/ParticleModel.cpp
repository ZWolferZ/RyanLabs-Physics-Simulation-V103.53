#include "ParticleModel.h"

void ParticleModel::AddRelativeForce(const Vector& force, const Vector& point)
{
	AddForce(force);
}