#pragma once
#include "Patterns/Singleton.h"

using kvproperty = std::pair<std::string, std::string>;

class PropertyManager : public Singleton<PropertyManager>
{
public:
	PropertyManager();
	virtual ~PropertyManager();

	bool Add(std::string propertyName, std::string propertyVal);

	bool GetBool(std::string propertyName, bool& propertyVal) const;
	bool GetInt(std::string propertyName, int& propertyVal) const;
	bool GetChar(std::string propertyName, char& propertyVal) const;
	bool GetString(std::string propertyName, std::string& propertyVal) const;
private:
	std::unordered_map<std::string, std::string> m_Properties;
};