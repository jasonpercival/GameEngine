#ifndef PARTICLECOMPONENT_H
#define PARTICLECOMPONENT_H

#include "Component.h"

class ParticleComponent : public Component
{
	// Inherited via Component
	virtual bool OnCreate(GameObject* parent_) override;
	virtual void Update(float deltaTime_) override;
};

#endif

