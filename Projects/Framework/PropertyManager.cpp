#include "FrameworkPCH.h"
#include "PropertyManager.h"


PropertyManager::PropertyManager()
{
}


PropertyManager::~PropertyManager()
{
}

bool PropertyManager::Add(std::string propertyName, std::string propertyVal)
{
	if (m_Properties.find(propertyName) != m_Properties.end())
	{
		return false;
	}
	
	m_Properties.insert(std::make_pair(propertyName, propertyVal));

	return true;
}

bool PropertyManager::GetBool(std::string propertyName, bool& propertyVal) const
{
	auto result{ m_Properties.find(propertyName) };
	if (result == m_Properties.end() || (result->second != "0" && result->second != "1"))
	{
		return false;
	}

	std::istringstream(result->second) >> propertyVal;

	return true;
}

bool PropertyManager::GetInt(std::string propertyName, int & propertyVal) const
{
	auto result{ m_Properties.find(propertyName) };
	if (result == m_Properties.end())
	{
		return false;
	}

	propertyVal = std::stoi(result->second);
	return true;
}

bool PropertyManager::GetFloat(std::string propertyName, float & propertyVal) const
{
	auto result{ m_Properties.find(propertyName) };
	if (result == m_Properties.end())
	{
		return false;
	}

	propertyVal = std::stof(result->second);
	return true;
}

bool PropertyManager::GetString(std::string propertyName, std::string & propertyVal) const
{
	auto result{ m_Properties.find(propertyName) };
	if (result == m_Properties.end())
	{
		return false;
	}

	propertyVal = result->second;
	return true;
}