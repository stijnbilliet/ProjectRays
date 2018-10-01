#include "FrameworkPCH.h"
#include "XMLTree.h"

XmlTree::XmlTree()
	:m_Stack{}
{
	m_Stack.push_back(new XmlNode());
}

XmlTree::~XmlTree()
{
	for (auto node : m_Stack)
	{
		safe_delete(node);
	}
	m_Stack.clear();
}

XmlNode* XmlTree::Insert(std::string& tagName, bool isOneLiner)
{
	XmlNode* newNode{ nullptr };
	if (!(m_Stack.back()->basedata.first == tagName))
	{
		newNode = new XmlNode();
		newNode->basedata.first = tagName;
		m_Stack.back()->children.push_back(newNode);
		if (!(isOneLiner))
		{
			m_Stack.push_back(newNode);
		}
	}
	else
	{
		newNode = m_Stack.back();
		m_Stack.pop_back();
	}
	return newNode;
}

XmlNode* XmlTree::GetRoot()
{
	return m_Stack.back();
}

XmlNode::~XmlNode()
{
	for (auto node : children)
	{
		safe_delete(node);
	}
}

std::vector<XmlNode*> XmlNode::Search(std::string && tagName)
{
	std::vector<XmlNode*> findings{};
	std::queue<XmlNode*> elems{};
	elems.push(this);

	XmlNode* current{};
	while (!empty(elems))
	{
		current = elems.front();
		if (current->basedata.first == tagName) findings.push_back(current);
		for (auto node : current->children)
		{
			elems.push(node);
		}
		elems.pop();
	}
	return findings;
}