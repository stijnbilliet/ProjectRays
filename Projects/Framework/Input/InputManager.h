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

	void OnInit() override;
	void AttachObserver(Observer* obs);
	void AddInputAction(SDL_Event event, Command* cmd, unsigned int plyIndex);
	void HandleInput(SDL_Event* e, unsigned int playerIndex);

private:
	//Keyboard/Mouse
	void EvaluateKeyCode(SDL_KeyboardEvent* e, unsigned int playerIndex);
	void EvaluateMouseClick(SDL_MouseButtonEvent* e, unsigned int playerIndex);

	//GamePad
	void InitGamepad();
	void FetchControllers();
	void CloseControllers();

	void EvaluateControllerButton(SDL_ControllerButtonEvent* e, unsigned int playerIndex);
	void EvaluateControllerAxis(SDL_ControllerAxisEvent* e, unsigned int playerIndex);
	void ProcessAxisMotion(SDL_ControllerAxisEvent*e, unsigned int playerIndex);

	std::unordered_multimap<unsigned int, std::pair<SDL_Event, Command*>> m_pInputs;
	std::vector<SDL_GameController*> m_pGamePads;
};