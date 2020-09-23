#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <glm/gtx/string_cast.hpp>

#include "../../Engine/Core/CoreEngine.h"
#include "../Test/TestComponentA.h"
#include "../Test/TestComponentB.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
private:
};

#endif // !GAMESCENE_H

