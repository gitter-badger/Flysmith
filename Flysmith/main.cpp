#include "FlysmithGame.h"
#include "Application\Timer.h"
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>

FORCE_NV_OPTIMUS

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	FlysmithGame game(hInstance);
	return game.Run();
}