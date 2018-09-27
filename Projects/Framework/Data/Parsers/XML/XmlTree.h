#pragma once
struct XmlNode
{
	~XmlNode();

	//Member vars
	std::pair<std::string, std::string> basedata;
	std::unordered_map<std::string, std::string> properties;
	std::vector<XmlNode*> children;

	//Member funct
	std::vector<XmlNode*> Search(std::string&& tagName);
};

class XmlTree
{
public:
	XmlTree();
	~XmlTree();

	XmlNode* Insert(std::string& tagName, bool isOneLiner);
	XmlNode* GetRoot();
private:
	std::vector<XmlNode*> m_Stack;
};