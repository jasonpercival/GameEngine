#ifndef TESTCOMPONENTA_H
#define TESTCOMPONENTA_H

#include "../../Engine/Rendering/3D/Component.h"

class TestComponentA : public Component
{
public:
	TestComponentA();
	virtual ~TestComponentA();
	bool OnCreate(GameObject* parent_) override;
	void Update(float deltaTime_) override;	
};

#endif // !TESTCOMPONENTA_H

