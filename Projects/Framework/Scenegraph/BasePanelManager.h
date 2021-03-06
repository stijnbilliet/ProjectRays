#pragma once
#include "Patterns\SingleInstance.h"
#include "Scenegraph\BasePanel.h"

template<typename T>
class BasePanelManager : public SingleInstance<BasePanelManager<T>>
{
public:
	BasePanelManager()
		:SingleInstance(), m_pCurPanel(nullptr)
	{

	};
	virtual ~BasePanelManager()
	{
		for (auto pPanel : m_pPanels)
		{
			safe_delete(pPanel);
		}
	};

	void SetActive(const std::string& name)
	{
		for (auto pPanel : m_pPanels)
		{
			if (pPanel->GetName() == name)
			{
				m_pCurPanel = pPanel;
				break;
			}
		}
	};

	const T* GetActive() const
	{
		return m_pCurPanel;
	}

	void Add(T* pPanel)
	{
		m_pPanels.push_back(pPanel);
	};

	/*Implementation of BaseObj*/
	virtual void OnInit(GameContext* pGameContext) override
	{
		if (m_pCurPanel)
		{
			m_pCurPanel->Init(pGameContext);
		}
	};

	virtual void OnUpdate(GameContext* pContext) override
	{
		if (m_pCurPanel)
		{
			m_pCurPanel->Update(pContext);
		}
	};

	virtual void OnDraw(GameContext* pContext) const override
	{
		if (m_pCurPanel)
		{
			m_pCurPanel->Draw(pContext);
		}
	};

protected:
	T* m_pCurPanel;
	std::list<T*> m_pPanels;
};