#include "TestComponentA.h"

TestComponentA::TestComponentA() : Component()
{
	// std::cout << "TestComponentA Constructor" << std::endl;
}

TestComponentA::~TestComponentA()
{
	// std::cout << "TestComponentA Destructor" << std::endl;
}

bool TestComponentA::OnCreate(GameObject* parent_)
{
	parent = parent_;
	// std::cout << "TestComponentA OnCreate" << std::endl;
	return true;
}

void TestComponentA::Update(float deltaTime_)
{
	//std::cout << "TestComponentA Update: deltaTime = " << deltaTime_ << std::endl;
}
