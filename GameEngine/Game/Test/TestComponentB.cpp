#include "TestComponentB.h"

TestComponentB::TestComponentB() : Component()
{
	// std::cout << "TestComponentB Constructor" << std::endl;
}

TestComponentB::~TestComponentB()
{
	// std::cout << "TestComponentB Destructor" << std::endl;
}

bool TestComponentB::OnCreate(GameObject* parent_)
{
	parent = parent_;
	// std::cout << "TestComponentB OnCreate" << std::endl;
	return true;
}

void TestComponentB::Update(float deltaTime_)
{
	std::cout << "TestComponentB Update: deltaTime = " << deltaTime_ << std::endl;
}
