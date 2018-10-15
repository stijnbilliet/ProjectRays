#pragma once

class GameContext;
class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	BaseObject(const BaseObject& other) = default;
	BaseObject(BaseObject&& other) = default;
	BaseObject& operator=(const BaseObject& other) = default;
	BaseObject& operator=(BaseObject&& other) = default;

	void Init();
	void Update(GameContext* pGameContext);
	void Draw(GameContext* pGameContext) const;

protected:
	virtual void PreInit();
	virtual void OnInit();
	virtual void PostInit();

	virtual void PreUpdate(GameContext* pGameContext);
	virtual void OnUpdate(GameContext* pGameContext);
	virtual void PostUpdate(GameContext* pGameContext);

	virtual void PreDraw(GameContext* pGameContext) const;
	virtual void OnDraw(GameContext* pGameContext) const;
	virtual void PostDraw(GameContext* pGameContext) const;
};