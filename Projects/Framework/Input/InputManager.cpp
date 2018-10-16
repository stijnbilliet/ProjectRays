#include "FrameworkPCH.h"
#include "InputManager.h"
#include "Patterns/Command.h"

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
	output.button.type = eType;

	if (eType == SDL_MOUSEBUTTONDOWN || eType == SDL_MOUSEBUTTONUP)
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
		safe_delete(kvPair.second);
	}
}

void InputManager::AddInputAction(SDL_Event e, Command* cmd)
{
	auto inputMapping = std::pair<SDL_Event, Command*>(e, cmd);
	m_pInputs.push_back(inputMapping);
}

void InputManager::AttachObserver(Observer* obs)
{
	if (obs)
	{
		for (auto kvPair : m_pInputs)
		{
			auto command = kvPair.second;
			command->addObserver(obs);
		}
	}
}

void InputManager::HandleInput(SDL_Event e)
{
	//Keyboard
	if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
	{
		EvaluateKeyCode(e.key);
	}

	//Mouse
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		EvaluateMouseClick(e.button);
	}
	
	if (e.type == SDL_MOUSEMOTION)
	{
		m_MousePos = glm::vec2(e.motion.x, e.motion.y);
	}
}

const glm::vec2 & InputManager::GetMousePos() const
{
	return m_MousePos;
}

void InputManager::EvaluateKeyCode(SDL_KeyboardEvent e)
{
	for (auto kvPair : m_pInputs)
	{
		auto event = kvPair.first.key;
		auto type = kvPair.first.type;
		if (e.type == type && e.keysym.sym == event.keysym.sym)
		{
			auto command = kvPair.second;
			command->execute();
			break;
		}
	}
}

void InputManager::EvaluateMouseClick(SDL_MouseButtonEvent e)
{
	for (auto kvPair : m_pInputs)
	{
		auto event = kvPair.first.button;
		auto type = kvPair.first.type;
		if (e.type == type && e.button == event.button)
		{
			auto command = kvPair.second;
			command->execute();
			break;
		}
	}
}