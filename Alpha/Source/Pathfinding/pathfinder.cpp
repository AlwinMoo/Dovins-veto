#include "pathfinder.h"

constexpr static unsigned int MAX_ITER{ 10000 };

PathManager::PathManager(game_map* Map)
{
	this->Map = Map;
	frontier = {};
}

PathManager::~PathManager()
{
}

std::string PathManager::VectorToString(AEVec2 pos)
{
	return std::string{ std::to_string(pos.x) + ',' + std::to_string(pos.y) };
}

bool PathManager::CheckCell(AEVec2 CurrPos, AEVec2 LastPos, AEVec2 GoalPos)
{
	if (!IsPositionValid(CurrPos)) // if the position is invalid
		return false;

	if (visited.count(VectorToString(CurrPos))) // if we have visited it
		return false;

	visited[VectorToString(CurrPos)] = LastPos; // if we haven't visited it before

	if (CurrPos.x == GoalPos.x && CurrPos.y == GoalPos.y) // are we at our goal
		return true;

	frontier.push(std::make_pair(AEVec2{ CurrPos.x, CurrPos.y + 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x + 1, CurrPos.y }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x, CurrPos.y - 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x - 1, CurrPos.y }, CurrPos));

	frontier.push(std::make_pair(AEVec2{ CurrPos.x + 1, CurrPos.y + 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x - 1, CurrPos.y - 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x - 1, CurrPos.y + 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x + 1, CurrPos.y - 1 }, CurrPos));

	return false;
}

bool PathManager::IsPositionValid(AEVec2 coord)
{
	return Map->map_arr[Map->GetIndex(coord.x, coord.y)] == game_map::TILE_TYPE::TILE_NONE || Map->map_arr[Map->GetIndex(coord.x, coord.y)] >= game_map::TILE_TYPE::NUM_TYPES_TILE || static_cast<int>(Map->map_arr[Map->GetIndex(coord.x, coord.y)]) < 0; // is it ground
}

std::vector<AEVec2> PathManager::GetPath(AEVec2 StartPos, AEVec2 GoalPos)
{
	if (!IsPositionValid(GoalPos))
		return std::vector<AEVec2>();

	std::vector<AEVec2> backtrackPath{};

	// clean up
	std::queue<std::pair<AEVec2, AEVec2>> empty{};
	std::swap(frontier, empty);
	visited.clear();

	frontier.push(std::make_pair(StartPos, AEVec2{-1, -1}));

	unsigned int iter = 0;
	while (frontier.size() > 0)
	{
		std::pair<AEVec2, AEVec2> cellInfo = frontier.front();
		frontier.pop();

		if (CheckCell(cellInfo.first, cellInfo.second, GoalPos))
			break;

		++iter;

		if (iter >= MAX_ITER) // if we have reached max iterations but still have yet to find the path
			return std::vector<AEVec2>();
	}

	AEVec2 currPos = GoalPos;

	// while the current position exists and the last position is not 0,0
	while (visited.count(VectorToString(currPos)) && (visited[VectorToString(currPos)].x != -1 && visited[VectorToString(currPos)].y != -1))
	{
		if (currPos.x != -1 && currPos.y != -1)
			backtrackPath.push_back(currPos);
		
		currPos = visited[VectorToString(currPos)];
	}

	std::reverse(backtrackPath.begin(), backtrackPath.end());

	return backtrackPath;
}
