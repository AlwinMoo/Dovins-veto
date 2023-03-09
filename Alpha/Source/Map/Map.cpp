#include "Map.h"
#include <cmath>
#include <iostream>

//CP_Image level_sprites[NUM_TYPES_TILE];
//CP_Image fog;

game_map::game_map(unsigned int width_size, unsigned int height_size, float world_width, float world_height, int grid_offset)
{
	this->width = width_size;
	this->height = height_size;
	this->map_size = width_size * height_size;
	this->map_arr = new TILE_TYPE[static_cast<size_t>(width_size) * height_size]{ TILE_TYPE::TILE_NONE };
	this->world_width = world_width;
	this->world_height = world_height;
	this->world_offset = grid_offset * this->GetTileSize();
	this->tile_offset = grid_offset;
}

int game_map::GetIndex(int x, int y)
{
	x -= tile_offset;
	return y * width + x;
}

AEVec2 game_map::GetWorldPos(int index)
{
	AEVec2 rtn{};
	AEVec2Set(&rtn, GetX(index) * this->GetTileSize() + world_offset + (this->GetTileSize() * 0.5), GetY(index) * this->GetTileSize() + (this->GetTileSize() * 0.5));
	
	return rtn;
}

float game_map::GetWorldX(int x)
{
	return x * this->GetTileSize() + (this->GetTileSize() * 0.5) + world_offset;
}

float game_map::GetWorldY(int y)
{
	return y * this->GetTileSize() + (this->GetTileSize() * 0.5);
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

	return(minXIndex >= 0 && minYIndex >= 0 && maxXIndex < width && maxYIndex < height);
}

AEVec2 game_map::SnapCoordinates(AEVec2 coord)
{
	AEVec2 rtn{};
	AEVec2Set(&rtn, std::floor(coord.x / GetTileSize()) * GetTileSize() + (GetTileSize() * 0.5), std::floor(coord.y / GetTileSize()) * GetTileSize() + (GetTileSize() * 0.5));

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
	return GetIndex(gridPos.x / GetTileSize(), gridPos.y / GetTileSize());
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

game_map::~game_map()
{
	delete[] this->map_arr;
	this->map_arr = nullptr;

	//delete this;
}

//void load_file(game_map* dst, const char* src)
//{
//	int len = dst->width * dst->width;
//	//dst->world_height = dst->world_width = (int)sqrt(len);
//	
//	for (int i = 0; i < len; ++i)
//	{
//		dst->arr[i] = src[i];
//	}
//	return;
//}

//void render_map(game_map* map, AEVec2 offset)
//{
//	for (unsigned int i = 0; i < map->height * map->width; ++i)
//	{
//		double gridsize = (double)(CP_System_GetWindowHeight() / map->height);
//		CP_Settings_ImageMode(CP_POSITION_CORNER);
//		CP_Image_Draw(level_sprites[map->arr[i] - 1], (float)(GetY((int)i, map->width) * gridsize) + offset.x, (float)(GetX(i, map->height) * (float)gridsize) + offset.y, (float)gridsize, (float)gridsize - 0.5f, 255);
//	}
//}
//
//void render_mapFog(game_map* map, AEVec2 offset, AEVec2 pos, int range, int mapoffset)
//{
//	double gridsize = CP_System_GetWindowHeight() / map->height;
//	CP_Settings_ImageMode(CP_POSITION_CORNER);
//	pos.x -= mapoffset;
//	for (unsigned int i = 0; i < map->height * map->width; ++i)
//	{
//		int y = GetX(i, map->width);
//		int x = GetY(i, map->height);
//		if (x >= pos.x - range && x <= pos.x + range)
//		{
//			if (y >= pos.y - range && y <= pos.y + range)
//			{
//				continue;
//			}
//		}
//		CP_Image_Draw(fog, (float)(GetY((int)i, map->width) * gridsize) + offset.x, (float)(GetX(i, map->height) * gridsize) + offset.y, (float)gridsize, (float)gridsize - 0.5f, 245);
//
//	}
//}
//
//void loadSprites(void)
//{
//	int i = 0;
//	while (i < NUM_TYPES_TILE - 1)
//	{
//		char tmp1[100] = FILEPATH; // path of file
//		char tmp2[100] = "tile_"; // file name without number 
//		char buffer[10]; // contain the number + file extention
//		snprintf(buffer, 10, "%d.png", i); // put number and file extention tgt: 0.png, 1.png
//		strcat(tmp2, buffer); // put tile_ and number and extension tgt: tile_0.png, tile_1.png
//		strcat(tmp1, tmp2); // put tgt with file path: Assets/tiles/tile_0.png
//		level_sprites[i] = CP_Image_Load(tmp1);
//		++i;
//	}
//	char tem[] = FILEPATH "tile_fog.png";
//
//
//	fog = CP_Image_Load(tem);
//}