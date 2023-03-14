#include "pathfinder.h"

constexpr static unsigned int MAX_ITER{ 10000 };

PathManager::PathManager(game_map* Map) : Map(Map)
{
	closedList.clear();
	for (int i = 0; i < Map->height; ++i)
	{
		for (int j = 0; j < Map->width; ++j)
		{
			closedList.insert(std::make_pair(VectorToString(AEVec2{ (float)i,(float)j }), false)); // add every single point in
		}
	}

	cellData.clear();
	for (int i = 0; i < Map->height; ++i)
	{
		for (int j = 0; j < Map->width; ++j)
		{
			cell empty;

			empty.parent = AEVec2{ (float)-1, (float)-1 };
			empty.f = FLT_MAX;
			empty.g = FLT_MAX;
			empty.h = FLT_MAX;

			cellData.insert(std::make_pair(VectorToString(AEVec2{ (float)i,(float)j }), empty)); // add every single point in
		}
	}
}

PathManager::~PathManager()
{
}

std::string PathManager::VectorToString(AEVec2 pos)
{
	return std::string{ std::to_string((int)pos.x) + ',' + std::to_string((int)pos.y) };
}

AEVec2 PathManager::StringToVector(std::string buf)
{
	AEVec2 result{ 0,0 };
	
	std::string num{};
	for (int i = 0; i < buf.size(); ++i)
	{
		if (buf[i] != ',')
		{
			num += buf[i];
		}
		else
		{
			result.x = std::stoi(num);
			num.clear();
			continue;
		}
	}
	result.y = std::stoi(num);

	return result;
}

bool PathManager::IsPositionValid(AEVec2 coord)
{
	if (coord.x < 0 || coord.y < 0 || coord.x >= Map->width || coord.y >= Map->height)
		return false;
	coord.x += Map->tile_offset;
	return Map->map_arr[Map->GetIndex(coord.x, coord.y)] == game_map::TILE_TYPE::TILE_NEXUS ||
		Map->map_arr[Map->GetIndex(coord.x, coord.y)] == game_map::TILE_TYPE::TILE_NONE ||
		Map->map_arr[Map->GetIndex(coord.x, coord.y)] >= game_map::TILE_TYPE::NUM_TYPES_TILE ||
		static_cast<int>(Map->map_arr[Map->GetIndex(coord.x, coord.y)]) < 0; // is it ground
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(AEVec2 coord, AEVec2 goal)
{
	// Return using the distance formula
	return AEVec2Distance(&coord, &goal);
}

bool PathManager::calculate_neighbour(AEVec2 parent, AEVec2 goalPos)
{
	AEVec2 NewPos{ 0,0 };	
	std::string sPos{};
	double fNew, gNew, hNew;

	// @TODO make sure path exists

#pragma region NORTH
	// x, y + 1
	NewPos = AEVec2{ parent.x, parent.y + 1};
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.0;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region SOUTH
	NewPos = AEVec2{ parent.x, parent.y - 1};
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.0;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region EAST
	NewPos = AEVec2{ parent.x + 1, parent.y };
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.0;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region WEST
	// x-1, y
	NewPos = AEVec2{ parent.x - 1, parent.y };
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.0;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region NORTHWEST
	NewPos = AEVec2{ parent.x - 1, parent.y + 1};
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.414;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region NORTHEAST
	NewPos = AEVec2{ parent.x + 1, parent.y + 1};
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.414;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region SOUTHWEST
	NewPos = AEVec2{ parent.x - 1, parent.y - 1 };
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.414;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

#pragma region SOUTHEAST
	NewPos = AEVec2{ parent.x + 1, parent.y - 1 };
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		// if destination is same as current
		if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
		{
			// set parent of goal cell
			cellData.at(sPos).parent = parent;
			result = tracePath(cellData, goalPos);
			return true;
		}
		else if (!closedList.at(sPos)) // if already on the closed list, or if not possible
		{
			gNew = cellData.at(VectorToString(parent)).g + 1.414;
			hNew = calculateHValue(NewPos, goalPos);
			fNew = gNew + hNew;

			// If it isn’t on the open list, add it to
			// the open list. Make the current square
			// the parent of this square. Record the
			// f, g, and h costs of the square cell
			//                OR
			// If it is on the open list already, check
			// to see if this path to that square is
			// better, using 'f' cost as the measure.

			if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
			{
				openList.insert(std::make_pair(fNew, sPos));

				// update details
				cellData.at(sPos).f = fNew;
				cellData.at(sPos).g = gNew;
				cellData.at(sPos).h = hNew;
				cellData.at(sPos).parent = parent;
			}
		}

	}
#pragma endregion

	return false;
}

std::vector<AEVec2> PathManager::tracePath(std::map<std::string, cell> cellData, AEVec2 goalPos)
{
	/*printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;*/
	AEVec2 temp = goalPos;

	std::stack<AEVec2> Path;
	std::vector<AEVec2> usablePath;

	while (!(cellData.at(VectorToString(temp)).parent.x == temp.x && cellData.at(VectorToString(temp)).parent.y == temp.y) && temp.x != -1.f && temp.y != -1.f)
	{
		Path.push(temp);
		temp = cellData.at(VectorToString(temp)).parent;
	}

	Path.push(temp);

	while (!Path.empty()) {
		AEVec2 top = Path.top();
		Path.pop();
		usablePath.emplace_back(top);
	}

	return usablePath;
}

std::vector<AEVec2> PathManager::GetPath(AEVec2 const& startPos, AEVec2 const& goalPos)
{
	if (!IsPositionValid(goalPos) || !IsPositionValid(startPos))
		return std::vector<AEVec2>();

	/*std::map<AEVec2, cell> cellData;
	std::map<AEVec2, bool> closedList;*/

	// @TODO take these out so that we dont have to initialise a new map every time AI searches for a path

	cellData.at(VectorToString(startPos)).f = 0.0;
	cellData.at(VectorToString(startPos)).g = 0.0;
	cellData.at(VectorToString(startPos)).h = 0.0;
	cellData.at(VectorToString(startPos)).parent = startPos;

	openList.clear();
	openList.insert(std::make_pair(0.0, VectorToString(startPos)));

	bool foundDest = false;
	while (!openList.empty())
	{
		// popping list.begin
		open_datapair curr = *openList.begin();
		openList.erase(openList.begin());

		// add coord to closed list since we are investigating now
		closedList.at(curr.second) = true;

		// Start searching neighbours

		for (int newX = -1; newX <= 1; newX++) 
		{
			for (int newY = -1; newY <= 1; newY++) 
			{
				double gNew, hNew, fNew;


				AEVec2 NewPos = AEVec2{ StringToVector(curr.second).x + newX, StringToVector(curr.second).y + newY };
				std::string sPos = VectorToString(NewPos);

				if (IsPositionValid(NewPos))
				{
					// if destination is same as current
					if (NewPos.x == goalPos.x && NewPos.y == goalPos.y)
					{
						// set parent of goal cell
						cellData.at(sPos).parent = StringToVector(curr.second);
						result = tracePath(cellData, goalPos);
						foundDest =  true;
					}
					else if (closedList.at(sPos) == false) // if already on the closed list, or if not possible
					{
						gNew = cellData.at(VectorToString(StringToVector(curr.second))).g + 1.0;
						hNew = calculateHValue(NewPos, goalPos);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add it to
						// the open list. Make the current square
						// the parent of this square. Record the
						// f, g, and h costs of the square cell
						//                OR
						// If it is on the open list already, check
						// to see if this path to that square is
						// better, using 'f' cost as the measure.

						if (cellData.at(sPos).f == FLT_MAX || cellData.at(sPos).f > fNew)
						{
							openList.insert(std::make_pair(fNew, sPos));

							// update details
							cellData.at(sPos).f = fNew;
							cellData.at(sPos).g = gNew;
							cellData.at(sPos).h = hNew;
							cellData.at(sPos).parent = StringToVector(curr.second);
						}
					}

				}
			}
		}

		//foundDest = calculate_neighbour(StringToVector(curr.second), goalPos);
		if (foundDest) // path is now valid
			return result;
	}

	return std::vector<AEVec2>();
}

//std::vector<AEVec2> PathManager::GetPath(AEVec2 StartPos, AEVec2 GoalPos)
//{
//	if (!IsPositionValid(GoalPos))
//		return std::vector<AEVec2>();
//
//	std::vector<AEVec2> backtrackPath{};
//
//	// clean up
//	std::queue<std::pair<AEVec2, AEVec2>> empty{};
//	std::swap(frontier, empty);
//
//	visited.clear();
//
//	frontier.push(std::make_pair(StartPos, AEVec2{-1, -1}));
//
//	unsigned int iter = 0;
//	while (frontier.size() > 0)
//	{
//		std::pair<AEVec2, AEVec2> cellInfo = frontier.front();
//		frontier.pop();
//
//		if (CheckCell(cellInfo.first, cellInfo.second, GoalPos))
//			break;
//
//		++iter;
//
//		if (iter >= MAX_ITER) // if we have reached max iterations but still have yet to find the path
//			return std::vector<AEVec2>();
//	}
//
//	AEVec2 currPos = GoalPos;
//
//	// while the current position exists and the last position is not 0,0
//	while (visited.count(VectorToString(currPos)) && (visited[VectorToString(currPos)].x != -1 && visited[VectorToString(currPos)].y != -1))
//	{
//		if (currPos.x != -1 && currPos.y != -1)
//			backtrackPath.push_back(currPos);
//		
//		currPos = visited[VectorToString(currPos)];
//	}
//
//	std::reverse(backtrackPath.begin(), backtrackPath.end());
//
//	return backtrackPath;
//}


