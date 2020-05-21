#pragma once

#include "Edge.h"

#include <memory>
#include <deque>

class Node
{
public:
	using LinksArray = std::deque<Edge>;

	explicit Node(unsigned int num);

	void setValue(unsigned int value);
	void setMarker(unsigned int marker);

	void addLink(unsigned int node, unsigned int linkValue = 0);
	bool removeLinkTo(unsigned int node);
	void clearLinks();

	unsigned int getNumber() const { return _number; }
	const LinksArray& getLinks() const { return _links; }
	unsigned int getMarker() const { return _marker; }

private:
	unsigned int _number;
	unsigned int _value;
	LinksArray  _links;
	unsigned int _marker = 0;
};




