#include "Map.h"

//CP_Image level_sprites[NUM_TYPES_TILE];
//CP_Image fog;

game_map::game_map(unsigned int width_size, unsigned int height_size, float world_width, float world_height)
{
	this->width = width_size;
	this->height = height_size;
	this->map_arr = new char[width_size * height_size + 1];
	this->map_arr[(width_size * height_size)] = '\0';
	this->world_width = world_width;
	this->world_height = world_height;
}

int game_map::get_index(int x, int y, int width)
{
	return x * width + y;
}

AEVec2 game_map::get_worldpos(int index)
{
	AEVec2 rtn{};
	AEVec2Set(&rtn, get_x(index, this->width) * (this->world_width / this->width), get_y(index, this->height) * (this->world_height / this->height));
	
	return rtn;
}

float game_map::get_world_x(int x)
{
	return x * (this->world_width / this->width);
}

float game_map::get_world_y(int y)
{
	return y * (this->world_height / this->height);
}

int game_map::get_y(int index, int height)
{
	return index % height;    // where "/" is an integer division;
}

int game_map::get_x(int index, int width)
{
	return index / width;    // % is the "modulo operator", the remainder of i / width;
}

float game_map::get_tile_size()
{
	return static_cast<float>(this->world_height / this->height);
}

game_map::~game_map()
{
	delete[] this->map_arr;
	this->map_arr = nullptr;

	delete this;
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
//		CP_Image_Draw(level_sprites[map->arr[i] - 1], (float)(get_y((int)i, map->width) * gridsize) + offset.x, (float)(get_x(i, map->height) * (float)gridsize) + offset.y, (float)gridsize, (float)gridsize - 0.5f, 255);
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
//		int y = get_x(i, map->width);
//		int x = get_y(i, map->height);
//		if (x >= pos.x - range && x <= pos.x + range)
//		{
//			if (y >= pos.y - range && y <= pos.y + range)
//			{
//				continue;
//			}
//		}
//		CP_Image_Draw(fog, (float)(get_y((int)i, map->width) * gridsize) + offset.x, (float)(get_x(i, map->height) * gridsize) + offset.y, (float)gridsize, (float)gridsize - 0.5f, 245);
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