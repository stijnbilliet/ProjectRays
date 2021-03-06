// Game.cpp : Defines the entry point for the console application.
//
#include "GamePCH.h"
#include "Gamemode/GMSandbox.h"

void StartHeapControl();

int main(int argc, char* argv[])
{
	try
	{
		std::vector<std::string> args{};
		for (int i=0; i < argc; ++i)
		{
			args.push_back(argv[i]);
		}

		auto pRenderer = new GL_Renderer();
		auto pRaytracer = new CL_Renderer();
		auto pGameMode = new GMSandbox();
		auto pGame = new Application(pGameMode, pRenderer, pRaytracer, args);

		pGame->Run();

		safe_delete(pGame);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
		system("pause");
	}
	return 0;
}

void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number

	//_CrtSetBreakAlloc(484);
#endif
}