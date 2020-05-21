#include "Node.h"

Node::Node(unsigned int num)
{
	_number = num;
	_value = 0;
}

void Node::setValue(unsigned int value)
{
	_value = value;
}

void Node::setMarker(unsigned int marker)
{
	_marker = marker;
}

void Node::addLink(unsigned int node, unsigned int linkValue)
{
	_links.emplace_back(Edge{ node, linkValue });
}

bool Node::removeLinkTo(unsigned int node)
{
	bool res = false;
	auto it_end = _links.end();
	for (auto it = _links.begin(); it != it_end; ++it)
	{
		if (it->getNode() == node)
		{
			_links.erase(it);
			res = true;
			break;
		}
	}
	return res;
}

void Node::clearLinks()
{
	_links.clear();
}
