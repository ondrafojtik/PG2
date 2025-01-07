#pragma once
#include "glm/vec2.hpp"
#include <vector>
#include <array>
#include <math.h>

#include <iostream>

static float tileSize = 128.0f;

static glm::vec2 grid_to_position(const glm::vec2& grid)
{
	glm::vec2 pos = glm::vec2((grid.x * tileSize), -grid.y * tileSize);
	//{ 128.0f + grid.x * 128.0f, (-grid.y * 128.0f) }
	return pos;
}

static glm::vec2 position_to_grid(const glm::vec2& position)
{
	glm::vec2 grid = glm::vec2(
		std::ceilf((position.x - tileSize) / tileSize),
		std::floorf(position.y / tileSize)
	);
	grid.y = glm::abs(grid.y - 7);
	return grid;
}

static int grid_to_i(const glm::vec2 grid)
{
	return ((int)grid.x + ((int)grid.y * 15));
}

struct Node
{
	bool collidable = false;
	bool visited = false;
	float global_cost = 0.0f;
	float local_cost = 0.0f;
	glm::vec2 position;
	//std::vector<Node> adj;
	std::vector<Node*> adj;
	Node* parent = nullptr;
	//glm::vec2 parentPos;
	//represents the "grid color" and whether its collidable or not
	char grid_type;
};

static bool isCollidable(char c)
{
	if ((c == 'F') || (c == 'V'))
		return true;
	return false;
}

static float distance(Node* lhs, Node* rhs)
{
	return std::sqrtf(std::powf(lhs->position.x - rhs->position.x, 2) + std::powf(lhs->position.y - rhs->position.y, 2));
}

struct Pathfinder 
{
	Node* nodes = nullptr;
	Node* start = nullptr;
	Node* end = nullptr;

	//for now its reset aswell
	void Init(std::array<Node, (15 * 8)>& grid)
	{
		for (Node& node : grid)
		{
			node.adj.reserve(4);
			if (isCollidable(node.grid_type))
				node.collidable = true;
			else
				node.collidable = false;
			node.global_cost = INFINITY;
			node.local_cost = INFINITY;
			//node.parentPos = { -1, -1 };
			node.visited = false;
			node.parent = nullptr;

			int x = (int)node.position.x;
			int y = (int)node.position.y;

			if (x > 0)
				node.adj.push_back(&grid[(x - 1) + ((y + 0) * 15)]);
			if (x < 15 - 1)
				node.adj.push_back(&grid[(x + 1) + ((y + 0) * 15)]);
			if (y > 0)
				node.adj.push_back(&grid[(x + 0) + ((y - 1) * 15)]);
			if (y < 8 - 1)
				node.adj.push_back(&grid[(x + 0) + ((y + 1) * 15)]);

		}
	}

	void Reset(std::array<Node, (15 * 8)>& grid)
	{
		for (Node& node : grid)
		{
			if (isCollidable(node.grid_type))
				node.collidable = true;
			else
				node.collidable = false;
			node.global_cost = INFINITY;
			node.local_cost = INFINITY;
			//node.parentPos = { -1, -1 };
			node.visited = false;
			node.parent = nullptr;

			node.adj.clear();

			int x = (int)node.position.x;
			int y = (int)node.position.y;

			if (x > 0)
				node.adj.push_back(&grid[(x - 1) + ((y + 0) * 15)]);
			if (x < 15 - 1)
				node.adj.push_back(&grid[(x + 1) + ((y + 0) * 15)]);
			if (y > 0)
				node.adj.push_back(&grid[(x + 0) + ((y - 1) * 15)]);
			if (y < 8 - 1)
				node.adj.push_back(&grid[(x + 0) + ((y + 1) * 15)]);

		}
	}

	void Solve(std::array<Node, (15 * 8)>& grid, Node* start, Node* end)
	{
		
		Reset(grid);

		if (start->collidable || end->collidable)
			return;

		Node* currNode = start;
		std::list<Node*> notTested;
		start->local_cost = 0.0f;
		start->global_cost = distance(start, end);
		notTested.push_back(start);

		while (!notTested.empty() && currNode != end)
		{
			//sort the list (so that theres higher chance of getting the correct one) 
			notTested.sort([](const Node* lhs, const Node* rhs) { return lhs->global_cost < rhs->global_cost; });
			
			while (!notTested.empty() && notTested.front()->visited)
				notTested.pop_front();
				
			if (notTested.empty())
				break;

			currNode = notTested.front();
			currNode->visited = true;

			for (auto adj : currNode->adj)
			{
				if (!adj->visited && !adj->collidable)
					notTested.push_back(adj);

				float lowerTest = currNode->local_cost + distance(currNode, end);
				if (lowerTest < adj->local_cost)
				{
					adj->parent = currNode;
					adj->local_cost = lowerTest;
					adj->global_cost = adj->local_cost + distance(adj, end);
				}
			}
		}
		currNode = nullptr;
		notTested.clear();
		//std::cout << "Algo over!\n";
	}

	~Pathfinder()
	{
		delete start;
		delete end;
		delete nodes;
	}

};


/*
struct Node
{
	bool collidable = false;
	bool visited = false;
	float global_cost = 0.0f;
	float local_cost = 0.0f;
	glm::vec2 position;
	//std::vector<Node> adj;
	std::vector<glm::vec2> adj;
	//Node* parent = nullptr;
	glm::vec2 parentPos;
	//represents the "grid color" and whether its collidable or not
	char grid_type;
};

//this is an A* type pathFinder

static bool isCollidable(char c)
{
	if ((c == 'F') || (c == 'V'))
		return true;
	return false;
}

static float distance(Node* lhs, Node* rhs)
{
	return std::sqrtf(std::powf(lhs->position.x - rhs->position.x, 2) + std::powf(lhs->position.y - rhs->position.y, 2));
}

struct Pathfinder
{
	Pathfinder() { std::cout << "Pathfinder init!\n"; }

	void Init(std::array<Node, (15 * 8)>& grid)
	{
		for (Node& node : grid)
		{
			node.adj.reserve(4);
			if (isCollidable(node.grid_type))
				node.collidable = true;
			else
				node.collidable = false;
			node.global_cost = INFINITY;
			node.local_cost = INFINITY;
			node.parentPos = { -1, -1 };
			node.visited = false;

			int x = (int)node.position.x;
			int y = (int)node.position.y;

			if (x > 0)
				node.adj.push_back(grid[(x - 1) + ((y + 0) * 15)].position);
			if (x < 15 - 1)
				node.adj.push_back(grid[(x + 1) + ((y + 0) * 15)].position);
			if (y > 0)
				node.adj.push_back(grid[(x + 0) + ((y - 1) * 15)].position);
			if (y < 8 - 1)
				node.adj.push_back(grid[(x + 0) + ((y + 1) * 15)].position);

		}
		
	}
	
	void Reset(std::array<Node, (15 * 8)>& grid)
	{
		for (auto node : grid)
		{
			if (isCollidable(node.grid_type))
				node.collidable = true;
			else
				node.collidable = false;
			node.global_cost = INFINITY;
			node.local_cost = INFINITY;
			node.parentPos = { -1, -1 };
			node.visited = false;

			int x = (int)node.position.x;
			int y = (int)node.position.y;

			if (x > 0)
				node.adj.push_back(grid[(x - 1) + ((y + 0) * 15)].position);
			if (x < 15 - 1)
				node.adj.push_back(grid[(x + 1) + ((y + 0) * 15)].position);
			if (y > 0)
				node.adj.push_back(grid[(x + 0) + ((y - 1) * 15)].position);
			if (y < 8 - 1)
				node.adj.push_back(grid[(x + 0) + ((y + 1) * 15)].position);

		}
	}
	
	//creates the path
	void Solve(std::array<Node, (15 * 8)>& grid, Node start, Node end)
	{
		std::vector<Node> notTestedNodes;
		
		Node currNode = start;
		currNode.local_cost = 0.0f;
		currNode.global_cost = distance(&currNode, &end);
		for (glm::vec2 nodePosition : currNode.adj)
		{
			glm::vec2 pos = position_to_grid(nodePosition);
			notTestedNodes.push_back(grid[grid_to_i(pos)]);
		}

		notTestedNodes.push_back(currNode);
		while (!notTestedNodes.empty() && currNode.position != end.position)
		{
			
			//sort the list (so that theres higher chance of getting the correct one) 
			bool sorted = false;
			while (!sorted)
			{
				sorted = true;
				for (int i = 0; i < notTestedNodes.size() - 1; i++)
					if (notTestedNodes[i].global_cost < notTestedNodes[i + 1].global_cost)
					{
						std::swap(notTestedNodes[i], notTestedNodes[i + 1]);
						sorted = false;
					}
			}

			while (!notTestedNodes.empty() && notTestedNodes[0].visited)
				notTestedNodes.erase(notTestedNodes.begin());

			if (notTestedNodes.empty())
				break;

			currNode = notTestedNodes[0];
			currNode.visited = true;

			for (glm::vec2 position : currNode.adj)
			{
				glm::vec2 _grid = position_to_grid(position);
				Node nodeNeighbour = grid[grid_to_i(_grid)];

				if (!nodeNeighbour.visited && !nodeNeighbour.collidable)
					notTestedNodes.push_back(nodeNeighbour);

				float lowerTest = currNode.local_cost + distance(&currNode, &nodeNeighbour);
				if (lowerTest < nodeNeighbour.local_cost)
				{
					nodeNeighbour.parentPos = currNode.position;
					nodeNeighbour.local_cost = lowerTest;
					nodeNeighbour.global_cost = nodeNeighbour.local_cost + distance(&nodeNeighbour, &end);
				}


			}

		}

		std::cout << "Found way!\n";
	}
	
};*/