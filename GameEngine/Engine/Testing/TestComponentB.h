#ifndef TESTCOMPONENTB_H
#define TESTCOMPONENTB_H

#include "../../Engine/Rendering/3D/Component.h"

class TestComponentB : public Component
{
public:
	TestComponentB();
	virtual ~TestComponentB();
	bool OnCreate(GameObject* parent_) override;
	void Update(float deltaTime_) override;	
};

#endif // !TESTCOMPONENTB_H

