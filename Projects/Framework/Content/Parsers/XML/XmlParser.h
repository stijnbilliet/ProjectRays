#pragma once
using stringPair = std::unordered_map<std::string, std::string>;

class XmlTree;
class XmlParser
{
public:
	explicit XmlParser();
	~XmlParser();

	XmlParser(const XmlParser& other) = delete;
	XmlParser& operator=(const XmlParser&) = delete;

	XmlTree* LoadFromDisk() const;
	
	void SetActiveFile(const std::string& fileName);
	void SetActiveFile(const std::string&& fileName);
private:
	stringPair GetProperties(std::string& line) const;
	std::ifstream* m_pFile;
};