#pragma once

class Renderer;
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
	void FixedUpdate(float stepRate);
	void Update(float deltaTime);
	void Draw(Renderer* pContext) const;

protected:
	virtual void PreInit();
	virtual void OnInit();
	virtual void PostInit();

	virtual void PreFixedUpdate(float stepRate);
	virtual void OnFixedUpdate(float stepRate);
	virtual void PostFixedUpdate(float stepRate);

	virtual void PreUpdate(float elapsedSec);
	virtual void OnUpdate(float elapsedSec);
	virtual void PostUpdate(float elapsedSec);

	virtual void PreDraw(Renderer* pContext) const;
	virtual void OnDraw(Renderer* pContext) const;
	virtual void PostDraw(Renderer* pContext) const;
};