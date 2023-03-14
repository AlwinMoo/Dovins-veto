#pragma once

#include <AEEngine.h>
#include <queue>
#include <vector>
#include <map>
#include <string>

#include <unordered_map>
#include <stack>
#include <set>
#include <sstream>

#include "../Map/Map.h"

// A structure to hold the necessary parameters
struct cell 
{
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	AEVec2 parent;
	// f = g + h
	double f, g, h;
};

class PathManager
{
	game_map* Map;
	std::queue<std::pair<AEVec2, AEVec2>> frontier;
	std::map < std::string, AEVec2 > visited;

	std::string VectorToString(AEVec2);
	AEVec2 StringToVector(std::string);

	bool CheckCell(AEVec2, AEVec2, AEVec2);

	// A* variables
	bool IsPositionValid(AEVec2);

	using closed_datapair = std::pair<std::string, bool>;
	using data_pair = std::pair<std::string, cell>;
	using open_datapair = std::pair<double, std::string>;

	std::map<std::string, cell> cellData;
	std::map<std::string, bool> closedList;
	std::set<std::pair<double, std::string>> openList;

	std::vector<AEVec2> result;

	bool calculate_neighbour(AEVec2, AEVec2);
	std::vector<AEVec2> tracePath(std::map < std::string , cell > , AEVec2);

public:
	
	//std::vector<AEVec2> GetPath(AEVec2, AEVec2);
	std::vector<AEVec2> GetPath(AEVec2 const&, AEVec2 const&);

	PathManager(game_map* Map);
	~PathManager();
};