/******************************************************************************/
/*!
\file		Map.cpp
\author		Alwin Moo
\author		Alvin Yeo
\par        email: moo.g@digipen.edu
\par		email: a.yeo@digipen.edu
\date       January 26, 2023
\brief		Alwin (Primary 50%), Alvin (Secondary 50%)
Contains definitions for a grid based map, including functions to convert
coordinates, edit the binary collision array and checking for collision.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Map.h"
#include <cmath>
#include <iostream>

game_map::game_map(unsigned int width_size, unsigned int height_size, float world_width, float world_height, int grid_offset)
{
	this->width = width_size;
	this->height = height_size;
	this->map_size = width_size * height_size;
	this->map_arr = new TILE_TYPE[static_cast<size_t>(map_size)]{ TILE_TYPE::TILE_NONE };
	this->world_width = world_width;
	this->world_height = world_height;
	this->world_offset = grid_offset * this->GetTileSize();
	this->tile_offset = grid_offset;
}

int game_map::GetIndex(int x, int y)
{
	return y * width + x;
}

AEVec2 game_map::GetWorldPos(int index)
{
	AEVec2 rtn{};
	AEVec2Set(&rtn, GetX(index) * this->GetTileSize() + world_offset + (this->GetTileSize() * 0.5f), GetY(index) * this->GetTileSize() + (this->GetTileSize() * 0.5f));
	
	return rtn;
}

float game_map::GetWorldX(int x)
{
	return x * this->GetTileSize() + (this->GetTileSize() * 0.5f) + world_offset;
}

float game_map::GetWorldY(int y)
{
	return y * this->GetTileSize() + (this->GetTileSize() * 0.5f);
}

bool game_map::IsInGrid(AEVec2 coord, int gridSizeX, int gridSizeY)
{
	int index = WorldToIndex(coord);
	if (index < 0)
		return false;

	int minXIndex = GetX(index);
	int minYIndex = GetY(index);
	int maxXIndex = minXIndex + gridSizeX - 1;
	int maxYIndex = minYIndex + gridSizeY - 1;

	return(minXIndex >= 0 && minYIndex >= 0 && static_cast<unsigned int>(maxXIndex) < width && static_cast<unsigned int>(maxYIndex) < height);
}

AEVec2 game_map::SnapCoordinates(AEVec2 coord)
{
	AEVec2 rtn{};
	AEVec2Set(&rtn, std::floor(coord.x / GetTileSize()) * GetTileSize() + (GetTileSize() * 0.5f), std::floor(coord.y / GetTileSize()) * GetTileSize() + (GetTileSize() * 0.5f));

	return rtn;
}

int game_map::GetY(int index)
{
	return index / this->width;    // where "/" is an integer division;
}

int game_map::GetX(int index)
{
	return (index % this->width);    // % is the "modulo operator", the remainder of i / width;
}

float game_map::GetTileSize()
{
	return static_cast<float>(this->world_height / this->height);
}

int game_map::WorldToIndex(AEVec2 pos)
{
	if (pos.x < world_offset || pos.x > world_offset + width * GetTileSize())
		return -1;

	AEVec2 gridPos = SnapCoordinates(pos);
	return GetIndex(static_cast<int>(gridPos.x / GetTileSize()) - tile_offset, static_cast<int>(gridPos.y / GetTileSize()));
}

void game_map::AddItem(TILE_TYPE tile, int index, int gridScalex, int gridScaleY)
{
	int minXIndex = GetX(index);
	int minYIndex = GetY(index);
	int maxXIndex = minXIndex + gridScalex - 1;
	int maxYIndex = minYIndex + gridScaleY - 1;

	for (int i = minXIndex; i <= maxXIndex; i++)
	{
		for (int j = minYIndex; j <= maxYIndex; j++)
			this->map_arr[GetIndex(i, j)] = tile;
	}
}

void game_map::RemoveItem(int index, int gridScalex, int gridScaleY)
{
	int minXIndex = GetX(index);
	int minYIndex = GetY(index);
	int maxXIndex = minXIndex + gridScalex - 1;
	int maxYIndex = minYIndex + gridScaleY - 1;

	for (int i = minXIndex; i <= maxXIndex; i++)
	{
		for (int j = minYIndex; j <= maxYIndex; j++)
			this->map_arr[GetIndex(i, j)] = TILE_TYPE::TILE_NONE;
	}
}

bool game_map::IsOccupied(int index, int gridScalex, int gridScaleY)
{
	if (index < 0)
		return true;

	int minXIndex = GetX(index);
	int minYIndex = GetY(index);
	int maxXIndex = minXIndex + gridScalex - 1;
	int maxYIndex = minYIndex + gridScaleY - 1;

	for (int i = minXIndex; i <= maxXIndex; i++)
	{
		for (int j = minYIndex; j <= maxYIndex; j++)
		{
			if (map_arr[GetIndex(i, j)] != TILE_TYPE::TILE_NONE)
			{
				if (map_arr[GetIndex(i, j)] >= TILE_TYPE::NUM_TYPES_TILE || (int)map_arr[GetIndex(i, j)] < 0)
					continue;
				return true;
			}
		}
	}
	return false;
}

int game_map::operator()(unsigned int const& x, unsigned int const& y)
{
	if (x > width || x < 0 || y > height || y < 0)
		return static_cast<int>(TILE_TYPE::TILE_NONE);

	switch (map_arr[y * width + x])
	{
	case game_map::TILE_TYPE::TILE_PLANET:
		return 10;
		break;
	/*case game_map::TILE_TYPE::TILE_NEXUS:
		break;
	case game_map::TILE_TYPE::NUM_TYPES_TILE:
		break;*/
	default:
		return 0;
		break;
	}
}

int game_map::operator()(AEVec2 const& pos)
{
	if (pos.x > width || pos.x < 0 || pos.y > height || pos.y < 0)
		return static_cast<int>(TILE_TYPE::TILE_NONE);

	switch (map_arr[static_cast<int>(pos.y * width + pos.x)])
	{
	case game_map::TILE_TYPE::TILE_PLANET:
		return 10;
		break;
	default:
		return 0;
		break;
	}
}

game_map::~game_map()
{
	delete[] this->map_arr;
	this->map_arr = nullptr;

	//delete this;
}