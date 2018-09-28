// Game.cpp : Defines the entry point for the console application.
//
#include "GamePCH.h"
#include "Gamemode/GMSandbox.h"

void StartHeapControl();

int main(int argc, char* argv[])
{
	try
	{
		StartHeapControl();

		auto pRenderer = new Renderer();
		auto pGameMode = new GMSandbox();
		auto pGame = new Game(pGameMode, pRenderer);

		pGame->Run();

		safe_delete(pGame);

		_CrtDumpMemoryLeaks();
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

	//_CrtSetBreakAlloc(285);
#endif
}