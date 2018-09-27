#include "FrameworkPCH.h"
#include "InputManager.h"
#include "Patterns/Command.h"

using Bind = std::pair<unsigned int, std::pair<SDL_Event, Command*>>;

SDL_Event EventFactory::CreateDesc(SDL_EventType eType, SDL_Keycode key)
{
	SDL_Event output{};
	output.type = eType;
	output.key.keysym.sym = key;
	return output;
}

SDL_Event EventFactory::CreateDesc(SDL_EventType eType, uint8_t button)
{
	SDL_Event output{};
	output.type = eType;

	if (eType == SDL_KEYDOWN || eType == SDL_KEYUP)
	{
		output.button.button = button;
	}
	else if (eType == SDL_CONTROLLERBUTTONDOWN || eType == SDL_CONTROLLERBUTTONUP)
	{
		output.cbutton.button = button;
	}

	return output;
}

InputManager::InputManager()
	:SingleInstance(), m_pInputs()
{
}

InputManager::~InputManager()
{
	for (auto kvPair : m_pInputs)
	{
		safe_delete(kvPair.second.second);
	}

	CloseControllers();
}

void InputManager::OnInit()
{
	InitGamepad();
}

void InputManager::AddInputAction(SDL_Event e, Command* cmd, unsigned int plyIndex)
{
	auto inputMapping = std::pair<SDL_Event, Command*>(e, cmd);
	auto playerMapping = Bind(plyIndex, inputMapping);

	m_pInputs.insert(playerMapping);
}

void InputManager::AttachObserver(Observer* obs)
{
	if (obs)
	{
		for (Bind kvPair : m_pInputs)
		{
			auto command = kvPair.second.second;
			command->addObserver(obs);
		}
	}
}

void InputManager::HandleInput(SDL_Event* e, unsigned int playerIndex)
{
	//Keyboard
	if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
	{
		EvaluateKeyCode(&e->key, playerIndex);
	}

	//Mouse
	if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		EvaluateMouseClick(&e->button, playerIndex);
	}

	//Gamepad
	if (e->type == SDL_CONTROLLERBUTTONDOWN || e->type == SDL_CONTROLLERBUTTONUP)
	{
		EvaluateControllerButton(&e->cbutton, playerIndex);
	}
}

void InputManager::EvaluateKeyCode(SDL_KeyboardEvent* e, unsigned int playerIndex)
{
	for (Bind kvPair : m_pInputs)
	{
		if (playerIndex == kvPair.first)
		{
			auto event = kvPair.second.first.key;
			auto type = kvPair.second.first.type;
			if (e->type == type && e->keysym.sym == event.keysym.sym)
			{
				auto command = kvPair.second.second;
				command->execute();
				break;
			}
		}
	}
}

void InputManager::EvaluateMouseClick(SDL_MouseButtonEvent* e, unsigned int playerIndex)
{
	for (Bind kvPair : m_pInputs)
	{
		if (playerIndex == kvPair.first)
		{
			auto event = kvPair.second.first.button;
			auto type = kvPair.second.first.type;
			if (e->type == type && e->button == event.button)
			{
				auto command = kvPair.second.second;
				command->execute();
				break;
			}
		}
	}
}

void InputManager::InitGamepad()
{
	//Init gamecontroller subsystem (gamecontroller is a higher level version of the joystick API)
	//this api will provide us with a bunch of axis mappings from popular controllers
	//(in contrary to the joystick api where we have to manually map everything)
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		throw std::runtime_error(std::string("SDL_InitSubSystem Error: SDL_INIT_JOYSTICK") + SDL_GetError());
	}

	FetchControllers();
}

void InputManager::FetchControllers()
{
	CloseControllers();

	int numControllers = SDL_NumJoysticks();
	for (int controllerIndex = 0; controllerIndex < numControllers; controllerIndex++)
	{
		if (SDL_IsGameController(controllerIndex))
		{
			m_pGamePads.push_back(SDL_GameControllerOpen(controllerIndex));
		}
	}
}

void InputManager::CloseControllers()
{
	for (auto controller : m_pGamePads)
	{
		SDL_GameControllerClose(controller);
	}
	m_pGamePads.clear();
}

void InputManager::EvaluateControllerButton(SDL_ControllerButtonEvent* e, unsigned int playerIndex)
{
	for (Bind kvPair : m_pInputs)
	{
		if (playerIndex == kvPair.first)
		{
			auto event = kvPair.second.first.cbutton;
			auto type = kvPair.second.first.type;
			if (e->type == type && e->button == event.button)
			{
				auto command = kvPair.second.second;
				command->execute();
				break;
			}
		}
	}
}

void InputManager::EvaluateControllerAxis(SDL_ControllerAxisEvent* e, unsigned int playerIndex)
{
	UNREFERENCED_PARAMETER(e);
	UNREFERENCED_PARAMETER(playerIndex);
}

void InputManager::ProcessAxisMotion(SDL_ControllerAxisEvent* e, unsigned int playerIndex)
{
	UNREFERENCED_PARAMETER(playerIndex);

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;

	char xDir = 0; char yDir = 0;
	//X axis motion
	if( e->axis == 0 )
	{
		//Left of dead zone
		if( e->value < -JOYSTICK_DEAD_ZONE )
		{
			xDir = -1;
		}
		//Right of dead zone
		else if( e->value > JOYSTICK_DEAD_ZONE )
		{
			xDir = 1;
		}
		else
		{
			xDir = 0;
		}
	}
	//Y axis motion
	else if( e->axis == 1 )
	{
		//Below of dead zone
		if( e->value < -JOYSTICK_DEAD_ZONE ) {
			yDir = -1;
		}
		//Above of dead zone
		else if(e->value > JOYSTICK_DEAD_ZONE )
		{
			yDir = 1;
		}
		else
		{
			yDir = 0;
		}
	}
}