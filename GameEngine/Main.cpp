#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"

int main(int argc, char* arg[])
{
	CoreEngine::GetInstance()->SetGameInterface(new Game1);

	if (!CoreEngine::GetInstance()->OnCreate("Game Engine", 1024, 768))
	{
		std::cout << "Failed to create game engine." << std::endl;
		return 0;
	}

	CoreEngine::GetInstance()->Run();
	return 0;
}
