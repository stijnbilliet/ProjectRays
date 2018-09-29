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