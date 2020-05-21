#pragma once

class Edge
{
	Edge();
public:
	Edge(unsigned int nodeNum, unsigned int value);

	unsigned int getValue() const { return _value; }
	unsigned int getNode() const { return _node; }

private:
	unsigned int _value;
	unsigned int  _node;
};
