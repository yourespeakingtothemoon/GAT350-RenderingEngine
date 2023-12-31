//#include "World01.h"
//#include "World02.h"
//#include "CheekyLittleDVDLogo.h"

//#include "World05.h"
#include "World06.h"
#include "World07.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Core/StringUtils.h"

using namespace std;

int main(int argc, char* argv[])
{
	

//	nc::ncString::signifier = 0;


	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("Assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World06>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
