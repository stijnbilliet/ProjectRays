#include "FrameworkPCH.h"
#include "XmlParser.h"
#include "XmlTree.h"

XmlParser::XmlParser()
	:m_pFile{}
{
}

XmlParser::~XmlParser()
{
	safe_delete(m_pFile);
}

void XmlParser::SetActiveFile(const std::string&& fileName)
{
	SetActiveFile(fileName);
}

void XmlParser::SetActiveFile(const std::string & fileName)
{
	m_pFile = new std::ifstream(fileName);
}

XmlTree* XmlParser::LoadFromDisk() const
{
	XmlTree* pDataTree{ new XmlTree() };
	std::string rawLine{};
	while (std::getline(*m_pFile, rawLine, '>'))
	{
		std::string::iterator tagBegin{ std::find(rawLine.begin(), rawLine.end(), '<') };
		if (tagBegin != rawLine.end())
		{
			//Tagname
			std::string::iterator tagnameBegin{ std::find_if(tagBegin, rawLine.end(), std::isalpha) };
			std::string::iterator tagnameEnd{ std::find_if(tagnameBegin, rawLine.end(), std::isspace) };
			std::string tagName{ tagnameBegin, tagnameEnd };

			//IsOneLiner (should push to stack)
			std::string quoteNeedle{ "\"" };
			std::string::iterator finalQuotePos{ std::find_end(rawLine.begin(), rawLine.end(), quoteNeedle.begin(), quoteNeedle.end()) };
			std::string::iterator oneLinerSlash{ std::find(finalQuotePos, rawLine.end(), '/') };
			bool isOneLiner{ oneLinerSlash != rawLine.end() };

			//Append to Tree
			XmlNode* insertedNode{ pDataTree->Insert(tagName, isOneLiner) };

			//Hook propeties && Values to Tree (if any)
			bool hasProperties{ finalQuotePos != rawLine.end() };
			bool hasValues{ std::find_if(rawLine.begin(), tagBegin, std::isalnum) != tagBegin };
			if (hasProperties)
			{
				std::string propLine{ tagnameEnd, finalQuotePos + 1 };
				insertedNode->properties = GetProperties(propLine);
			}
			else if (hasValues && !isOneLiner)
			{
				std::string newValues{ rawLine.begin(), tagBegin };
				newValues.erase(std::remove_if(newValues.begin(), newValues.end(), std::isspace), newValues.end());
				insertedNode->basedata.second = { newValues };
			}
		}
	}
	return pDataTree;
}

stringPair XmlParser::GetProperties(std::string & line) const
{
	std::unordered_map<std::string, std::string> newProperties{};

	std::string::iterator equalToPos{};
	std::string::iterator propertyNameBegin{};
	std::string::iterator beginQuotePos{ line.begin() };
	std::string::iterator endQuotePos{ beginQuotePos };

	while (beginQuotePos != line.end())
	{
		propertyNameBegin = std::find_if(endQuotePos, line.end(), std::isalnum);
		equalToPos = std::find(propertyNameBegin, line.end(), '=');
		beginQuotePos = std::find(equalToPos, line.end(), '\"');
		
		if (beginQuotePos != line.end())
		{
			endQuotePos = std::find(beginQuotePos + 2, line.end(), '\"');

			std::string propertyName{ propertyNameBegin, equalToPos };
			std::string propertyVal{ beginQuotePos + 1, endQuotePos };
			newProperties[propertyName] = propertyVal;
		}
	}
	return newProperties;
}