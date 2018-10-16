#pragma once
#include "Patterns/Subject.h"
#include "Patterns/SingleInstance.h"

class EventFactory
{
public:
	static SDL_Event CreateDesc(SDL_EventType eType, SDL_Keycode key);
	static SDL_Event CreateDesc(SDL_EventType eType, uint8_t button);
};

class Command;
class InputManager : public SingleInstance<InputManager>
{
public:
	InputManager();
	virtual ~InputManager();

	InputManager(const InputManager& other) = delete;
	InputManager(InputManager&& other) noexcept = delete;
	InputManager& operator=(const InputManager& other) = delete;
	InputManager& operator=(InputManager&& other) noexcept = delete;

	void AttachObserver(Observer* obs);
	void AddInputAction(SDL_Event event, Command* cmd);
	void HandleInput(SDL_Event e);

	const glm::vec2& GetMousePos() const;

private:
	//Keyboard/Mouse
	void EvaluateKeyCode(SDL_KeyboardEvent e);
	void EvaluateMouseClick(SDL_MouseButtonEvent e);

	std::vector<std::pair<SDL_Event, Command*>> m_pInputs;
	glm::vec2 m_MousePos;
};