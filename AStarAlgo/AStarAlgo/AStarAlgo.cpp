
#include "Node.h"

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <list>

void createNodes(std::vector<std::unique_ptr<Node>>& list, unsigned int n)
{
	for (unsigned int i = 0; i < n; ++i)
	{
		list.emplace_back(std::make_unique<Node>(i));
	}
}

void resetNodesMarkers(const std::vector<std::unique_ptr<Node>>& list)
{
	for (auto& ptr : list)
	{
		ptr->setMarker(0);
	}
}

// Поиск в ширину
void algoBFS(const std::vector<std::unique_ptr<Node>>& list, unsigned int startNode)
{
	std::cout << "BFS: ";

	std::queue<unsigned int> _nodesQ;
	_nodesQ.push(startNode);

	while (_nodesQ.size())
	{
		unsigned int nodeNum = _nodesQ.front();
		_nodesQ.pop();
		std::cout << ", " << nodeNum;

		const auto& links = list[nodeNum]->getLinks();
		for (auto& link : links)
		{
			auto linkedNode = link.getNode();
			if (list[linkedNode]->getMarker() == 0)
			{
				list[linkedNode]->setMarker(1);
				_nodesQ.push(linkedNode);
			}
		}
	}
	std::cout << std::endl;
}

// Поиск в глубину рекурсивный

void algoDFS_sub(const std::vector<std::unique_ptr<Node>>& list, unsigned int startNode)
{
	std::cout << "DFS: " << startNode << std::endl;
	list[startNode]->setMarker(1);

	for (auto& ln : list[startNode]->getLinks())
	{
		auto linkedNode = ln.getNode();
		if (list[linkedNode]->getMarker() == 0)
		{
			algoDFS_sub(list, linkedNode);
		}
	}
}

void algoDFS(const std::vector<std::unique_ptr<Node>>& list)
{
	for (const auto& pNode : list)
	{
		if (pNode->getMarker() == 0)
		{
			algoDFS_sub(list, pNode->getNumber());
		}
	}
}


// Поиск в глубину не рекурсивный
void algoDFSnr(const std::vector<std::unique_ptr<Node>>& list)
{
	std::cout << "DFS:";
	std::stack<std::pair<unsigned int, unsigned int>> stack_;

	for (const auto& pNode : list)
	{
		if (pNode->getMarker() == 0)
		{
			stack_.push(std::make_pair(pNode->getNumber(), 0));
			pNode->setMarker(1);
		}

		while (stack_.size())
		{
			std::pair<unsigned int, unsigned int> pr = stack_.top();
			stack_.pop();
			if (pr.second == 0)
			{
				std::cout << " " << pr.first;
			}

			auto links = list[pr.first]->getLinks();
			for (unsigned int i = pr.second; i < links.size(); ++i)
			{
				auto linkedNode = links[i].getNode();
				if (list[linkedNode]->getMarker() == 0)
				{
					stack_.push(std::make_pair(pr.first, i + 1));
					stack_.push(std::make_pair(linkedNode, 0));
					list[linkedNode]->setMarker(1);
					break;
				}
			}
		}
	}
	std::cout << std::endl;
}

// Минимальный остов (алгоритм Прима)
void algoMinOstov_Prima(const std::vector<std::unique_ptr<Node>>& list)
{
	std::cout << "Ostov: \n";

	unsigned int node = 0;
	std::vector<unsigned int> treeNodes;

	treeNodes.push_back(node);

	std::vector<std::pair<unsigned int, Edge>> edges;

	size_t ls_size = list.size();
	while (treeNodes.size() != ls_size)
	{
		edges.clear();
		for (auto& nodeNum : treeNodes)
		{
			for (auto& link : list[nodeNum]->getLinks())

			{
				auto it = std::find(treeNodes.begin(), treeNodes.end(), link.getNode());
				if (it == treeNodes.end())
				{
					edges.push_back(std::make_pair(nodeNum, link));
				}
			}
		}

		if (edges.empty())
		{
			break;
		}

		auto* smallestEdge = &edges[0];
		for (auto& pr : edges)
		{
			if (smallestEdge->second.getValue() > pr.second.getValue())
			{
				smallestEdge = &pr;
			}
		}

		treeNodes.push_back(smallestEdge->second.getNode());
		std::cout << smallestEdge->first << " - " << smallestEdge->second.getNode() << std::endl;
	}
	
}

void algoDeikstra(const std::vector<std::unique_ptr<Node>>& list, unsigned int startNode)
{
	std::list<unsigned int> allNodes;
	
	std::transform(list.begin(), list.end(), std::back_inserter(allNodes), [](const std::unique_ptr<Node>& pNode) {
		return pNode->getNumber();
	});
	
	unsigned int node = startNode;
	while (true) 
	{
		unsigned int baseMarker = list[node]->getMarker();
		for (const auto& link : list[node]->getLinks())
		{
			unsigned int selectedNode = link.getNode();

			// is node live?
			auto it = std::find(allNodes.begin(), allNodes.end(), selectedNode);
			if (it == allNodes.end())
			{
				continue;
			}

			unsigned int selectedNodeMarker = list[selectedNode]->getMarker();
			unsigned int sumMarker = link.getValue() + baseMarker;
			if (selectedNodeMarker == 0 || selectedNodeMarker > sumMarker)
			{
				list[selectedNode]->setMarker(sumMarker);
			}
		}

		// remove startNode
		allNodes.remove(node);
		if (allNodes.empty())
		{
			break;
		}

		// find min node
		unsigned int minNode = *allNodes.begin();
		for (const auto& nodeNum : allNodes)
		{
			if (list[nodeNum]->getMarker() != 0 && list[minNode]->getMarker() > list[nodeNum]->getMarker())
			{
				minNode = nodeNum;
			}
		}
		node = minNode;
	}

	for (const auto& pNode : list)
	{
		std::cout << "Node " << pNode->getNumber() << " val: " << pNode->getMarker() << std::endl;
	}
}

void addDoubleLink(std::vector<std::unique_ptr<Node>>& list, unsigned int nodeA, unsigned int nodeB, unsigned int value)
{
	list[nodeA]->addLink(nodeB, value);
	list[nodeB]->addLink(nodeA, value);
}

void clearGraph(std::vector<std::unique_ptr<Node>>& list)
{
	for (auto& pNode : list)
	{
		pNode->clearLinks();
		pNode->setMarker(0);
	}
}

void setGraph1(std::vector<std::unique_ptr<Node>>& list)
{
	/*
		  0
		/ \ \
	   1  2  3
		\ |  | \
		 4   5  6
		 |  /   |
		  7     8
		  |    /
		   \  /
			 9
	*/

	list[0]->addLink(1, 0);
	list[0]->addLink(2, 0);
	list[0]->addLink(3, 0);

	list[1]->addLink(4, 0);
	list[2]->addLink(4, 0);
	list[3]->addLink(5, 0);
	list[3]->addLink(6, 0);

	list[4]->addLink(7, 0);
	list[5]->addLink(7, 0);
	list[6]->addLink(8, 0);

	list[7]->addLink(9, 0);
	list[8]->addLink(9, 0);
}

void setGraph2(std::vector<std::unique_ptr<Node>>& list)
{
	/*
		  0
		/ \ \
	   1  2  3
		\ |//| \
		 4   5  6
		    /   |
	       7    8
		  |    /
		   \  /
			 9
	*/

	list[0]->addLink(1, 0);
	list[0]->addLink(2, 0);
	list[0]->addLink(3, 0);

	list[1]->addLink(4, 0);
	list[2]->addLink(4, 0);
	list[3]->addLink(5, 0);
	list[3]->addLink(6, 0);

	list[4]->addLink(3, 0);

	list[5]->addLink(7, 0);
	list[6]->addLink(8, 0);

	list[7]->addLink(9, 0);
	list[8]->addLink(9, 0);
}

void setGraph2_2(std::vector<std::unique_ptr<Node>>& list)
{
	/*
		  0
		/ \ \
	   1  2  3
		\ |//| \
		 4   5  6
			/   |
		   7    8
		  |    /
		   \  /
			 9
	*/

	addDoubleLink(list, 0, 1, 1);
	addDoubleLink(list, 0, 2, 1);
	addDoubleLink(list, 0, 3, 1);

	addDoubleLink(list, 1, 4, 6);
	addDoubleLink(list, 2, 4, 6);

	addDoubleLink(list, 3, 4, 7);
	addDoubleLink(list, 3, 5, 3);
	addDoubleLink(list, 3, 6, 3);


	addDoubleLink(list, 5, 7, 4);
	addDoubleLink(list, 6, 8, 6);

	addDoubleLink(list, 7, 9, 5);
	addDoubleLink(list, 8, 9, 5);
}


// Not complete!
// All presented algos worked: BFS, DFS, DFXnr, MinOstov, Deikstra

int main()
{
	std::vector<std::unique_ptr<Node>> list;
	createNodes(list, 10);

	setGraph2(list);

	// Algos
	std::cout << "------------------------\n";
	resetNodesMarkers(list);
	algoBFS(list, 0);

	std::cout << "------------------------\n";
	resetNodesMarkers(list);
	algoDFS(list);

	std::cout << "------------------------\n";
	resetNodesMarkers(list);
	algoDFSnr(list);

	clearGraph(list);
	setGraph2_2(list);

	std::cout << "------------------------\n";

	algoMinOstov_Prima(list);

	std::cout << "------------------------\n";

	clearGraph(list);
	setGraph2_2(list);

	algoDeikstra(list, 0);

}