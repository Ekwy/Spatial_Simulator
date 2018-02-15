#include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include "SDLGLContext.h"
#include "Stage.h"
#include "MainMenuScene.h"
#include "BuildScene.h"
#include "LiftOffScene.h"
#include "MissionReportScene.h"
#include "PutIntoOrbitScene.h"
#include "DebugScene.h"
#include "LandingScene.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)) return -1;
	if (TTF_Init()) return -1;

	SDLGLContext::setContext("Projet d'Intégration", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	GameData::loadData("Materials.txt", "Pieces.txt");

	Stage::sceneFactory["MainMenuScene"] = &Stage::createInstance<MainMenuScene>;
	Stage::sceneFactory["BuildScene"] = &Stage::createInstance<BuildScene>;
	Stage::sceneFactory["LiftOffScene"] = &Stage::createInstance<LiftOffScene>;
	Stage::sceneFactory["MissionReportScene"] = &Stage::createInstance<MissionReportScene>;
	Stage::sceneFactory["PutIntoOrbitScene"] = &Stage::createInstance<PutIntoOrbitScene>;
    Stage::sceneFactory["LandingScene"] = &Stage::createInstance<LandingScene>;
	Stage::sceneFactory["DebugScene"] = &Stage::createInstance<DebugScene>;

	Stage::setCurrentScene("MainMenuScene");

	{
		using namespace std::chrono;

		steady_clock::time_point lastTime = steady_clock::now();
		steady_clock::time_point currTime(lastTime), lastFramePost(lastTime);
		nanoseconds lag(0), nsPerFrame(static_cast<int>(1'000'000'000. / FPS));
		//int frames = 0;
		bool running = true;

		while (running) {
			/*if (((currTime = steady_clock::now()) - lastFramePost) >= 1s) {
				SDLGLContext::setTitle(std::to_string(frames) + " fps");
				frames = 0;

				lastFramePost = steady_clock::now();
			}*/

			lag += (currTime = steady_clock::now()) - lastTime;    //En réactivant le conteur fps, enlever "= now()"
			lastTime = currTime;

			EventDispatcher::pollEvents(running);

			while (lag >= nsPerFrame) {
				Stage::update();

				lag -= nsPerFrame;
			}

			if (running) {
				std::this_thread::sleep_until(currTime + nsPerFrame);

				Stage::draw();
				//frames++;
			}
		}
	}

	delete Singleton<Camera>::getInstance();

	TTF_Quit();
	SDL_Quit();
	return 0;
}