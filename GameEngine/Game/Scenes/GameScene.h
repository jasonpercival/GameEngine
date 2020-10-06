#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <glm/gtx/string_cast.hpp>

#include "../../Engine/Core/CoreEngine.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void Draw();

private:
};

#endif // !GAMESCENE_H

