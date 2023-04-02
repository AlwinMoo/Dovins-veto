/******************************************************************************/
/*!
\file		Map.h
\author		Alwin Moo
\author		Alvin Yeo
\par        email: moo.g@digipen.edu
\par		email: a.yeo@digipen.edu
\date       January 26, 2023
\brief		Alwin (Primary 50%), Alvin (Secondary 50%)
Contains declarations for a grid based map, including functions to convert
coordinates, edit the binary collision array and checking for collision.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <AEEngine.h>

class game_map
{
	public:
	/*!
	 * @brief destructor to be called in exit
	*/
	~game_map();
	game_map();

	enum class TILE_TYPE : int
	{
		TILE_NONE = 0, // THIS REPRESENTS EMPTY SPACE
		TILE_PLANET,
		TILE_NEXUS,
		NUM_TYPES_TILE
	};

	/*!
	 * @brief initialises out parameter (out_obj) with default values and makes sure it exists
	 * @param out_obj empty game_map object to be passed in so that default values can be written in and passed back
	 * @param width_size unsigned integer total number of units in the width of the grid
	 * @param height_size unsigned integer total number of units in the height of the grid
	 * @param world_width float max world width
	 * @param world_height float max world height
	*/
	game_map(unsigned int width_size, unsigned int height_size, float world_width, float world_height, int use_offset);
	
	unsigned int map_size;
	TILE_TYPE* map_arr; //1D char map array

	unsigned int width; //grid total width
	unsigned int height; //grid total height

	float world_width; //grid max world width
	float world_height; //grid max world height

	float world_offset;
	int tile_offset;

	/*!
	 * @brief converts from 1D array index to 2D array x-axis
	 * @param index 1D array index
	 * @param size integer max width of map
	 * @return integer x-axis in 2D array
	*/
	int GetX(int index);

	/*!
	 * @brief converts from 1D array index to 2D array y-axis
	 * @param index 1D array index
	 * @param size
	 * @return integer y-axis in 2D array
	*/
	int GetY(int index);

	/*!
	 * @brief converts from 2D grid coordinates to 1D array index
	 * @param x integer x-coord of grid
	 * @param y integer y-coord of grid
	 * @param width width of grid
	 * @return integer index of 1D array
	*/
	int GetIndex(int x, int y);

	/*!
	 * @brief Calculates the world position using 1D array index
	 * @param index integer index of 1D array
	 * @param in_map map object to access map information
	 * @return AEVec2 of position in world positioning
	*/
	AEVec2 GetWorldPos(int index);

	/*!
	 * @brief Calculates the world x-coord position using grid x-coord
	 * @param x integer x-coord in grid
	 * @param in_map map object to access map information
	 * @return float of position of x-axis
	*/
	float GetWorldX(int x);

	/*!
	 * @brief Calculates the world x-coord position using grid x-coord
	 * @param y integer y-coord in grid
	 * @param in_map map object to access map information
	 * @return float of position of x-axis
	*/
	float GetWorldY(int y);

	bool IsInGrid(AEVec2 coord, int gridSizeX=1, int gridSizeY=1);

	AEVec2 SnapCoordinates(AEVec2);

	float GetTileSize();

	/*!
	 * @brief Converts world pos to index, returns -1 if invalid position
	 *
	 * @param pos	- World pos
	 * @return		- Index in grid
	*/
	int WorldToIndex(AEVec2 pos);

	/*!
	 * @brief Converts world pos to PRE OFFSET index, returns -1 if invalid position
	 *
	 * @param pos	- World pos
	 * @return		- Pre X offset index in grid
	*/
	int WorldToPreOffsetIndex(AEVec2 pos);

	/*!
	 * @brief Adds an item into the map based on index
	 * 
	 * @param tile			- Type of tile to put in the map
	 * @param index			- Index to place tile
	 * @param gridScaleX	- X Scale of new item
	 * @param gridScaleY	- Y scale of new item
	*/
	void AddItem(TILE_TYPE tile, int index, int gridScalex = 1, int gridScaleY = 1);

	/*!
	 * @brief Removes an item into the map based on index
	 *
	 * @param tile			- Type of tile to put in the map
	 * @param index			- Index to place tile
	 * @param gridScaleX	- X Scale of new item
	 * @param gridScaleY	- Y scale of new item
	*/
	void RemoveItem(int index, int gridScalex = 1, int gridScaleY = 1);

	/*!
	 * @brief	Checks if an index is occupied. Scale is checked centered
	 *			on top left, ex, scaleX of 3 will check index and 2 tile
	 *			to the right of index
	 * 
	 * @param index			- Index to check
	 * @param gridScaleX	- X Scale to check
	 * @param gridScaleY	- Y scale to check
	*/
	bool IsOccupied(int index, int gridScalex=1, int gridScaleY=1);

	int operator()(unsigned int const&, unsigned int const&);
	int operator()(AEVec2 const&);
	//AEVec2 operator[](unsigned int const&);

};

//void load_file(game_map* dst, const char* src);

//void loadSprites(void);
//
//void render_map(game_map* map, AEVec2 offset);
//
//void render_mapFog(game_map* map, AEVec2 offset, AEVec2 pos, int range, int mapoffset);