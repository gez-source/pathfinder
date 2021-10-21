#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "Vector3.h"
#include "PathNode.h"
#include "PriorityQueue.h"
#include "FibonacciHeap.h"

#include <unordered_set>
#include <map>

float minDistanceToGoal = 0;// 0.000001f;// 0.1f;
float timeOutNotNearGoal = 3.0f;
int nodeOpenpathCost = 0;

bool DebugSolutionSpace = false;
bool DebugBestSolution = true;

bool navigatingGraph = false;

class Level
{
private:
    int width;
    int height;
    std::string grid;
public:
    Level(std::string g, int w, int h)
    {
        this->width = w;
        this->height = h;
        this->grid = g;
    }

    bool HasCollision(int x, int y, int z)
    {
        if ((x < 0 || x >= width) || (z < 0 || z >= height))
        {
            return true;
        }

        int index = x + (width * z);
        char c = grid[index];
        return (c == '#' || c == '@');
    }

    void FillPoint(Vector3 point, char fillValue)
    {
        int x = point.x;
        int y = point.y;
        int z = point.z;

        if ((x < 0 || x > width) || (z < 0 || z > height))
        {
            return;
        }

        int index = x + (width * z);

        grid[index] = fillValue;
    }

    void DrawLevel()
    {
        for (int y = 0; y <= height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = x + (width * y);

                std::cout << grid[index];

                if (x == width - 1) 
                {
                    std::cout << std::endl;
                }
            }
        }
    }
};

Level* currentLevel = nullptr;

enum PathfindingMethod
{
    Graph3D = 0,
    Graph2D = 1
};

PathfindingMethod pathfindingType = PathfindingMethod::Graph2D;

std::unordered_set<std::string>* closedSet = new std::unordered_set<std::string>();
std::map<PathNode*, std::vector<PathNode*>> parents;
std::map<std::string, PathNode*> nodeAtlas;

class ShortestPathResult
{
public:
    std::vector<Vector3>* shortestPath = new std::vector<Vector3>();
    std::vector<PathNode*>* Nodes = new std::vector<PathNode*>();
};

bool Contains(std::unordered_set<std::string>* unorderedSet, std::string itemInput)
{
    std::unordered_set<std::string>::const_iterator got = unorderedSet->find(itemInput);

    if (got == unorderedSet->end())
    {
        return false;
    }
    else
    {
        //std::cout << *got << " is in unorderedSet";
        return true;
    }
}

/// <summary>
/// The HeuristicCostEstimate awards the path finder the closer it gets to its goal.
/// </summary>
/// <param name="n">
/// The current node to be cost-benefit analysed.
/// </param>
/// <param name="goal">
/// The goal the path finder must locate.
/// </param>
/// <returns>
/// An arbitrary cost weight to be used for decision making by the path finder.
/// </returns>
float HeuristicCostEstimate(PathNode* n, Vector3 startNode, Vector3 goal)
{
    float collisionWeight = 0;
    if (currentLevel->HasCollision(n->x, n->y, n->z))
    {
        collisionWeight = 100000000;
    }
    return collisionWeight +
        Vector3::ManhattanDistance(n->Position, goal);
        //Vector3::Distance(n->Position, goal);
        //+ NavigationWeight(n, startNode, goal) // Increase the cost of the path if there are issues in navigation in the area.
       // + n.RiskIndicator();  // Increase the cost of the path given how dangerous the area is.
}

/// <summary>
/// Create a new PathNode for the specified position in grid-space.
/// 
/// If a node already exists for the position then a lookup is performed to reuse existing nodes.
/// </summary>
PathNode* CreateNode(int x, int y, int z)
{
    std::string keyInx = std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);

    if (nodeAtlas.count(keyInx) > 0)
    {
        return nodeAtlas[keyInx];
    }

    Vector3 position = Vector3(x, y, z);

    PathNode* n = new PathNode();

    n->Index = keyInx;
    n->DistFromStart = std::numeric_limits<float>::max();
    n->DistToGoal = std::numeric_limits<float>::max();
    n->Debug = DebugSolutionSpace;
    n->Position = position;
    n->x = x;
    n->y = y;
    n->z = z; 
    n->IsSolid = currentLevel->HasCollision(x, y, z);

    nodeAtlas.insert(std::pair<std::string, PathNode*>(keyInx, n));
    //nodeCounter++;

    return n;
}

/// <summary>
/// Get a PathNode for the specified position in world-space.
/// </summary>
PathNode* GetNodeAt(Vector3 point)
{
    return CreateNode((int)point.x, (int)point.y, (int)point.z);
}

/// <summary>
/// Get a list of adjacent nodes surrounding the specified grid-space position.
/// If the pathfindingType is set to Graph2D then just get adjacent nodes on the (x,0,z) 2D plane.
/// If the pathfindingType is set to Graph3D then get adjacent nodes on the (x, y, z) three dimensional cube.
/// </summary>
std::vector<PathNode*>* AdjacencyList(int x, int y, int z)
{
    std::vector<PathNode*>* adj = new std::vector<PathNode*>();

    // NODE CENTER.
    adj->push_back(CreateNode(x, y, z));
    // . . .
    // .[.].
    // . . .

    // CROSS.
    adj->push_back(CreateNode(x - 1, y, z));
    adj->push_back(CreateNode(x + 1, y, z));
    adj->push_back(CreateNode(x, y, z - 1));
    adj->push_back(CreateNode(x, y, z + 1));
    //  . [.] .
    // [.] . [.]
    //  . [.] .

    // CORNERS.
    //adj->push_back(CreateNode(x + 1, y, z + 1));
    //adj->push_back(CreateNode(x - 1, y, z + 1));
    //adj->push_back(CreateNode(x + 1, y, z - 1));
    //adj->push_back(CreateNode(x - 1, y, z - 1));
    // [.].[.]
    //  . . .
    // [.].[.] 

    if (pathfindingType == PathfindingMethod::Graph3D)
    {
        // CROSS.
        adj->push_back(CreateNode(x, y + 1, z));
        adj->push_back(CreateNode(x - 1, y + 1, z));
        adj->push_back(CreateNode(x + 1, y + 1, z));
        adj->push_back(CreateNode(x, y + 1, z - 1));
        adj->push_back(CreateNode(x, y + 1, z + 1));

        adj->push_back(CreateNode(x, y - 1, z));
        adj->push_back(CreateNode(x - 1, y - 1, z));
        adj->push_back(CreateNode(x + 1, y - 1, z));
        adj->push_back(CreateNode(x, y - 1, z - 1));
        adj->push_back(CreateNode(x, y - 1, z + 1));

        // CORNERS.
        adj->push_back(CreateNode(x + 1, y + 1, z + 1));
        adj->push_back(CreateNode(x + 1, y + 1, z + 1));
        adj->push_back(CreateNode(x - 1, y + 1, z - 1));
        adj->push_back(CreateNode(x - 1, y + 1, z - 1));

        adj->push_back(CreateNode(x + 1, y - 1, z + 1));
        adj->push_back(CreateNode(x + 1, y - 1, z + 1));
        adj->push_back(CreateNode(x - 1, y - 1, z - 1));
        adj->push_back(CreateNode(x - 1, y - 1, z - 1));
    }

    return adj;
}

std::vector<PathNode*>* AdjacencyList(PathNode* n)
{
    return AdjacencyList(n->x, n->y, n->z);
}

void EnqueueAdjacent(Vector3 startPosition, Vector3 goalPosition, PathNode* current, std::vector<PathNode*>*& neighbours, FibonacciHeap<float>* priorityQueue)
{
    int i;
    PathNode* child;
    float tentativeDist;

    // NAVIGATING
    // priorityQueue automatically removes item from queue when it is dequed
    //openSet.Remove(current);
    //priorityQueue->Extract_min();

    closedSet->insert(current->Index);

    // Get each neighbour proximal to the current node.
    neighbours = AdjacencyList(current);

    for (i = 0; i < neighbours->size(); i++)
    {
        child = (*neighbours)[i];

        if (!Contains(closedSet, child->Index))// && !child->IsSolid) // !closedSet.Contains(child->Index))
        {
            // Node has not already been visited.

            //tentativeDist = current->DistFromStart + Vector3::Distance(current->Position, child->Position);
            tentativeDist = current->DistFromStart + HeuristicCostEstimate(current, current->Position, child->Position);

            if (tentativeDist < child->DistFromStart)
            {
                // Great a better path, record it for later.

                if (parents.count(child) > 0)
                {
                    parents[child].push_back(current);
                }
                else
                {
                    parents.insert(std::pair<PathNode*, std::vector<PathNode*>>(child, std::vector<PathNode*>()));
                    parents[child].push_back(current);
                }

                //child->DistFromStart = tentativeDist;
                //child->DistToGoal = child->DistFromStart + HeuristicCostEstimate(child, startPosition, goalPosition);
            }

            child->DistFromStart = tentativeDist;
            child->DistToGoal = child->DistFromStart + HeuristicCostEstimate(child, startPosition, goalPosition);

            if (!Contains(closedSet, child->Index)) //!openSet.Contains(child))
            {
                priorityQueue->enqueue((void*)child, child->DistToGoal);
                //openSet.push_back(child); // This is a new node that needs processing.
            }
        }
    }
}


ShortestPathResult* AddSolution(PathNode* current)
{
    // FOUND GOAL
    // at this point we have all the solutions to the goal
    // so we pick the top most which is the best-first solution.
    ShortestPathResult* result = new ShortestPathResult();
    result->shortestPath->push_back(current->Position);
    result->Nodes->push_back(current);

    //TODO: Get parent hierarchy CHECK this is correctly getting ascendants
    PathNode* _parent;
    //if (parents.ContainsKey(current))
    if(parents.count(current) > 0)
    {
        //std::map<PathNode*, std::vector<PathNode*>> p = (*);
        _parent = parents[current].front();

        while (_parent != nullptr)
        {
            result->shortestPath->push_back(_parent->Position);
            result->Nodes->push_back(_parent);

            if (parents.count(_parent) > 0)
            {
                _parent = parents[_parent].front();
            }
            else
            {
                _parent = nullptr;
            }
        }

    }

    return result;
}

/// <summary>
/// Finds the shortest path from startPosition to the specified goalPosition.
/// </summary>
/// <returns>
/// Returns a list of intermediate points running along a single path to the best-first solution. 
/// </returns>
std::vector<Vector3>* FindShortestPath(Vector3 startPosition, Vector3 goalPosition, std::vector<PathNode*>*& shortestNodes)
{
    FibonacciHeap<float>* priorityQueue = new FibonacciHeap<float>();
    //PriorityQueue* priorityQueue = new PriorityQueue();

    std::vector<Vector3>* path = nullptr; // A set of points representing the shortest path to follow. Eventually will contain the most efficient path, and by default there's no solution.
    PathNode* current;
    int i;
    std::vector<PathNode*>* neighbours;
    float tentativeDist = 0;
    float actualDistnceToGoal;

    PathNode* startNode = GetNodeAt(startPosition);
    PathNode* goalNode = GetNodeAt(goalPosition);
    goalNode->IsGoal = true;

    startNode->DistFromStart = 0; // distance from start to start is 0.
    startNode->DistToGoal = HeuristicCostEstimate(startNode, startNode->Position, goalPosition);
    
    priorityQueue->enqueue(startNode, 0); // add to queue for processing

    navigatingGraph = true;

    // A* BEST-FIRST GRAPH SEARCH
    while (!priorityQueue->isEmpty() && navigatingGraph)
    {
        //openSet = BestFirst(openSet); //TODO: implement a priority queue so we don't have to sort every time.
        //current = openSet[0]; // the first node in the open set with the lowest distance to the goal.

        current = (PathNode*)priorityQueue->dequeue();

        if (current == nullptr) break;

        actualDistnceToGoal = HeuristicCostEstimate(current, current->Position, goalNode->Position); // Vector3::Distance(current->Position, goalNode->Position);


        if (actualDistnceToGoal <= minDistanceToGoal)
        {
            auto result = AddSolution(current);

            shortestNodes = result->Nodes;
            path = result->shortestPath;

            // EXIT CONDITION
            navigatingGraph = false;
        }
        else
        {
            EnqueueAdjacent(startPosition, goalPosition, current, neighbours, priorityQueue);
        }

    }

    delete priorityQueue;

    return path;
}

int randInt(int min, int max)
{
    int random = rand() % max + min;
    return random;
}

int main()
{
	std::cout << "Pathfinder by Gerallt Franke" << std::endl;

    srand(time(NULL));  //Changed from rand(). srand() seeds rand for you.

    std::string g = 
        "@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@"
        "@   @   @       @     @     @ @"
        "@ @ @ @ @ @@@@@ @ @ @@@ @ @ @ @"
        "@ @ @ @   @   @   @ @   @ @   @"
        "@ @ @ @@@@@ @ @ @@@@@ @@@ @@@ @"
        "@ @   @   @ @   @     @ @   @ @"
        "@ @@@@@ @ @ @@@@@ @@@@@ @@@ @@@"
        "@       @ @ @   @ @       @   @"
        "@ @@@@@@@ @@@ @ @ @ @@@@@ @@@ @"
        "@     @   @   @   @ @     @   @"
        "@@@@@ @ @ @ @@@@@@@ @ @@@@@ @@@"
        "@   @ @ @ @     @   @     @ @ @"
        "@@@ @ @ @@@@@@@ @@@@@ @@@ @ @ @"
        "@   @ @       @     @ @   @   @"
        "@ @@@ @@@ @ @@@@@@@ @ @ @ @@@@@"
        "@ @   @ @ @ @   @   @ @ @   @ @"
        "@ @ @@@ @ @@@ @ @ @@@@@ @@@ @ @"
        "@ @ @   @ @   @ @ @   @   @   @"
        "@ @ @@@ @ @ @@@ @ @ @ @@@ @@@ @"
        "@ @   @ @ @   @ @ @ @     @ @ @"
        "@ @@@ @ @ @@@ @ @ @ @@@@@@@ @ @"
        "@       @     @     @         @"
        "@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@";
    Level* level = new Level(g, 31, 22);
    currentLevel = level;

    //Vector3 startPosition = Vector3(15, 0, 22);
    //Vector3 goalPosition = Vector3(15, 0, 0);
    Vector3 startPosition = Vector3(randInt(0, 31), 0, randInt(0, 22));
    Vector3 goalPosition = Vector3(randInt(0, 31), 0, randInt(0, 22));

    std::vector<PathNode*>* pathNodes;
    std::vector<Vector3>* pathPoints;

    //currentLevel->FillPoint(startPosition, 'S');
    //currentLevel->FillPoint(goalPosition, 'G');
    //currentLevel->DrawLevel();

    pathPoints = FindShortestPath(startPosition, goalPosition, pathNodes);

    for (int i = 0; i < pathPoints->size(); i++)
    {
        Vector3 point = (*pathPoints)[i];

        currentLevel->FillPoint(point, '.');
    }
    currentLevel->FillPoint(startPosition, 'S');
    currentLevel->FillPoint(goalPosition, 'G');

    currentLevel->DrawLevel();

	return 0;
}