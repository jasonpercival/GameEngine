#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"

int main(int argc, char* arg[])
{
	// default engine settings
	std::string windowName = "Game Engine";
	int windowWidth = 1280, windowHeight = 720;
	int fps = 30;

	// load configuration from json file to overide default engine settings (if available)
	std::ifstream configFile("engineConfig.json");
	if (configFile)
	{
		std::cout << "Loading configuration settings.\n";
		json j;
		configFile >> j;
		fps = j["fps"].get<int>();
		windowName = j["window"]["name"].get<std::string>();
		windowWidth = j["window"]["width"].get<int>();
		windowHeight = j["window"]["height"].get<int>();
	}

	CoreEngine::GetInstance()->SetGameInterface(new Game1);
	CoreEngine::GetInstance()->SetFPS(fps);

	if (!CoreEngine::GetInstance()->OnCreate(windowName, windowWidth, windowHeight))
	{
		std::cout << "Failed to create game engine.\n";
		return 0;
	}

	CoreEngine::GetInstance()->Run();
	return 0;
}
