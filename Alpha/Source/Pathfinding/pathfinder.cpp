#include "pathfinder.h"

constexpr static unsigned int MAX_ITER{ 1000 };

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
	/*
		if !can_move_to_spot(cur_pos):
		return false
		if str(cur_pos) in visited:
			return false
	
		visited[str(cur_pos)] = last_pos
		if cur_pos.x == goal_pos.x and cur_pos.y == goal_pos.y:
			return true
		queue.push_back({"pos": {"x": cur_pos.x, "y": cur_pos.y + 1}, "last_pos": cur_pos})
		queue.push_back({"pos": {"x": cur_pos.x + 1, "y": cur_pos.y}, "last_pos": cur_pos})
		queue.push_back({"pos": {"x": cur_pos.x, "y": cur_pos.y - 1}, "last_pos": cur_pos})
		queue.push_back({"pos": {"x": cur_pos.x - 1, "y": cur_pos.y}, "last_pos": cur_pos})
		return false
	*/

	if (!IsPositionValid(CurrPos)) // if the position is invalid
		return false;

	//for (auto &pos : visited)
	//{
	//	if ((CurrPos.x == pos.first.x && CurrPos.y == pos.first.y))
	//	{
	//		return false;
	//	}
	//}

	if (visited.count(VectorToString(CurrPos))) // if we have visited it
		return false;

	visited[VectorToString(CurrPos)] = LastPos; // if we haven't visited it before

	if (CurrPos.x == GoalPos.x && CurrPos.y == GoalPos.y) // are we at our goal
		return true;

	frontier.push(std::make_pair(AEVec2{ CurrPos.x, CurrPos.y + 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x + 1, CurrPos.y }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x, CurrPos.y - 1 }, CurrPos));
	frontier.push(std::make_pair(AEVec2{ CurrPos.x - 1, CurrPos.y }, CurrPos));

	return false;
}

bool PathManager::IsPositionValid(AEVec2 coord)
{
	//return Map->map_arr[Map->get_index(Map->get_x(coord.x), Map->get_y(coord.y))] == 0; // is it ground

	return true;
}

std::vector<AEVec2> PathManager::GetPath(AEVec2 StartPos, AEVec2 GoalPos)
{
	/*
		if !can_move_to_spot(goal_pos):
		return []
		queue = []
		visited = {}
		queue.push_back({"pos": start_pos, "last_pos": null})
		var iters = 0
		while queue.size() > 0:
			var cell_info = queue.pop_front()
			if check_cell(cell_info.pos, cell_info.last_pos, goal_pos):
				break
			iters += 1
			if iters >= MAX_ITERS:
				return []
		var backtraced_path = []
		var cur_pos = goal_pos
		while str(cur_pos) in visited and visited[str(cur_pos)] != null:
			if cur_pos != null:
				backtraced_path.append(cur_pos)
			cur_pos = visited[str(cur_pos)]
		backtraced_path.invert()
		return backtraced_path
	*/

	// pathtrace to the goal grid
	// then when we are in the grid, lerp to absolute mouse position

	if (!IsPositionValid(GoalPos))
		return std::vector<AEVec2>();

	std::vector<AEVec2> backtrackPath{};

	// clean up
	frontier = {};

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
