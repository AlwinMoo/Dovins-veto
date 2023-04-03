/******************************************************************************/
/*!
\file		pathfinder.cpp
\author		Alwin Moo
\par        email: moo.g@digipen.edu
\date       January 29, 2023
\brief		Alwin (Primary 100%)
			A* pathfinding functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "pathfinder.h"

const static unsigned int MAX_ITER{ 10000 };

PathManager::PathManager(game_map* Map, bool player) : Map(Map), is_player(player), iterations(0)
{
	closedList.clear();
	for (unsigned int i = 0; i < Map->height; ++i)
	{
		for (unsigned int j = 0; j < Map->width; ++j)
		{
			closedList.insert(std::make_pair(VectorToString(AEVec2{ (float)i,(float)j }), false)); // add every single point in
		}
	}

	cellData.clear();
	for (unsigned int i = 0; i < Map->height; ++i)
	{
		for (unsigned int j = 0; j < Map->width; ++j)
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

/**
 * \brief convert from AEVec2 to string to be able to hash
 * 
 * \param pos coordinates to convert
 * \return formatted string of coordinates
 */
std::string PathManager::VectorToString(AEVec2 pos)
{
	return std::string{ std::to_string((int)pos.x) + ',' + std::to_string((int)pos.y) };
}

/**
 * \brief convert from string to AEvec2
 * 
 * \param string to convert
 * \return position from string
 */
AEVec2 PathManager::StringToVector(std::string buf)
{
	AEVec2 temp{ 0,0 };
	
	std::string num{};
	for (int i = 0; i < buf.size(); ++i)
	{
		if (buf[i] != ',')
		{
			num += buf[i];
		}
		else
		{
			temp.x = std::stof(num);
			num.clear();
			continue;
		}
	}
	temp.y = std::stof(num);

	return temp;
}

/**
 * \brief checks whether the position is valid
 * 
 * \param coord position to check
 * \return true position is valid
 * \return false position is invalid
 */
bool PathManager::IsPositionValid(AEVec2 coord)
{
	if (coord.x < 0 || coord.y < 0 || static_cast<unsigned int>(coord.x) >= Map->width || static_cast<unsigned int>(coord.y) >= Map->height)
		return false;

	if (!is_player)
		return 
			Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))] != game_map::TILE_TYPE::TILE_NONE ||
			Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))] < game_map::TILE_TYPE::NUM_TYPES_TILE ||
			static_cast<int>(Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))]) < 0; // is it ground
	else
		return
			Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))] == game_map::TILE_TYPE::TILE_NONE ||
			Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))] >= game_map::TILE_TYPE::NUM_TYPES_TILE ||
			static_cast<int>(Map->map_arr[Map->GetIndex(static_cast<int>(coord.x), static_cast<int>(coord.y))]) < 0; // is it ground
}

/**
 * \brief A Utility Function to calculate the 'h' heuristics.
 * 
 * \param coord starting position
 * \param goal ending position
 * \return returns the distance between 2 positons
 */
double calculateHValue(AEVec2 coord, AEVec2 goal)
{
	// Return using the distance formula
	return AEVec2Distance(&coord, &goal);
}

/**
 * \brief unrolled calculation of neighbour
 * 
 * \param parent parent position
 * \param goalPos position GO wishes to get to
 * \return true we have reached the goal
 * \return false we have yet to reach the goal
 */
bool PathManager::calculate_neighbour(AEVec2 parent, AEVec2 goalPos)
{
	AEVec2 NewPos{ 0,0 };	
	std::string sPos{};
	double fNew, gNew, hNew;

#pragma region NORTH
	// x, y + 1
	NewPos = AEVec2{ parent.x, parent.y + 1};
	sPos = VectorToString(NewPos);

	if (IsPositionValid(NewPos))
	{
		++iterations;
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
		++iterations;
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
		++iterations;
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
		++iterations;
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

	if (IsPositionValid(NewPos) && (Map->map_arr[Map->GetIndex(static_cast<int>(parent.x), static_cast<int>(parent.y) + 1)] == game_map::TILE_TYPE::TILE_NONE || Map->map_arr[Map->GetIndex(static_cast<int>(parent.x) - 1, static_cast<int>(parent.y))] == game_map::TILE_TYPE::TILE_NONE))
	{
		++iterations;
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

	if (IsPositionValid(NewPos) && (Map->map_arr[Map->GetIndex(static_cast<int>(parent.x), static_cast<int>(parent.y) + 1)] == game_map::TILE_TYPE::TILE_NONE || Map->map_arr[Map->GetIndex(static_cast<int>(parent.x) + 1, static_cast<int>(parent.y))] == game_map::TILE_TYPE::TILE_NONE))
	{
		++iterations;
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

	if (IsPositionValid(NewPos) && (Map->map_arr[Map->GetIndex(static_cast<int>(parent.x), static_cast<int>(parent.y) - 1)] == game_map::TILE_TYPE::TILE_NONE || Map->map_arr[Map->GetIndex(static_cast<int>(parent.x) - 1, static_cast<int>(parent.y))] == game_map::TILE_TYPE::TILE_NONE))
	{
		++iterations;
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

	if (IsPositionValid(NewPos) && (Map->map_arr[Map->GetIndex(static_cast<int>(parent.x), static_cast<int>(parent.y) - 1)] == game_map::TILE_TYPE::TILE_NONE || Map->map_arr[Map->GetIndex(static_cast<int>(parent.x) + 1, static_cast<int>(parent.y))] == game_map::TILE_TYPE::TILE_NONE))
	{
		++iterations;
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

/**
 * \brief calculates the final path backwards from calculated results
 * 
 * \param posData data of the current cell to start generating from
 * \param goalPos position GO wishes to get to
 * \return finalised path
 */
std::vector<AEVec2> PathManager::tracePath(std::map<std::string, cell> posData, AEVec2 goalPos)
{
	AEVec2 temp = goalPos;

	std::stack<AEVec2> Path;
	std::vector<AEVec2> usablePath;

	unsigned int itr{};

	while (!(posData.at(VectorToString(temp)).parent.x == temp.x && posData.at(VectorToString(temp)).parent.y == temp.y) && temp.x != -1.f && temp.y != -1.f)
	{
		Path.push(temp);
		temp = posData.at(VectorToString(temp)).parent;

		++itr;
		if (itr >= Map->map_size)
			return std::vector<AEVec2>();
	}

	Path.push(temp);

	while (!Path.empty()) {
		AEVec2 top = Path.top();
		Path.pop();
		usablePath.emplace_back(top);
	}

	return usablePath;
}

/**
 * \brief calculate the path using weighted A*
 * 
 * \param startPos position GO is at currently
 * \param goalPos position GO is wishes to get to
 * \return vector of AEVec2 containing grid based waypoints
 */
std::vector<AEVec2> PathManager::GetPath(AEVec2 const& startPos, AEVec2 const& goalPos)
{
	if (!IsPositionValid(goalPos) || !IsPositionValid(startPos))
		return std::vector<AEVec2>();

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

		foundDest = calculate_neighbour(StringToVector(curr.second), goalPos);

		if (iterations >= MAX_ITER)
			break;

		if (foundDest) // path is now valid
			return result;
	}

	return std::vector<AEVec2>();
}
