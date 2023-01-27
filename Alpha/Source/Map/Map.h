#include <AEEngine.h>

class game_map
{
	public:
	/*!
	 * @brief destructor to be called in exit
	*/
	~game_map();
	game_map();

	/*!
	 * @brief initialises out parameter (out_obj) with default values and makes sure it exists
	 * @param out_obj empty game_map object to be passed in so that default values can be written in and passed back
	 * @param width_size unsigned integer total number of units in the width of the grid
	 * @param height_size unsigned integer total number of units in the height of the grid
	 * @param world_width float max world width
	 * @param world_height float max world height
	*/
	game_map(unsigned int width_size, unsigned int height_size, float world_width, float world_height, bool use_offset);
	
	unsigned int map_size;
	char* map_arr; //1D char map array

	unsigned int width; //grid total width
	unsigned int height; //grid total height

	float world_width; //grid max world width
	float world_height; //grid max world height

	bool use_offset;
	float world_offset;
	int tile_offset;

	/*!
	 * @brief converts from 1D array index to 2D array x-axis
	 * @param index 1D array index
	 * @param size integer max width of map
	 * @return integer x-axis in 2D array
	*/
	int get_x(int index);

	/*!
	 * @brief converts from 1D array index to 2D array y-axis
	 * @param index 1D array index
	 * @param size
	 * @return integer y-axis in 2D array
	*/
	int get_y(int index);

	/*!
	 * @brief converts from 2D grid coordinates to 1D array index
	 * @param x integer x-coord of grid
	 * @param y integer y-coord of grid
	 * @param width width of grid
	 * @return integer index of 1D array
	*/
	int get_index(int x, int y);

	/*!
	 * @brief Calculates the world position using 1D array index
	 * @param index integer index of 1D array
	 * @param in_map map object to access map information
	 * @return AEVec2 of position in world positioning
	*/
	AEVec2 get_worldpos(int index);

	/*!
	 * @brief Calculates the world x-coord position using grid x-coord
	 * @param x integer x-coord in grid
	 * @param in_map map object to access map information
	 * @return float of position of x-axis
	*/
	float get_world_x(int x);

	/*!
	 * @brief Calculates the world x-coord position using grid x-coord
	 * @param y integer y-coord in grid
	 * @param in_map map object to access map information
	 * @return float of position of x-axis
	*/
	float get_world_y(int y);

	bool is_in_grid(AEVec2 coord);

	AEVec2 snap_coordinates(AEVec2);

	float get_tile_size();

};

//enum TILE_TYPE
//{
//	DEFAULT = 0, // THIS REPRESENTS EMPTY SPACE
//	GROUND_1 = 1, // THIS IS A NORMAL GROUND TILE
//	GROUND_2,
//	GROUND_3,
//	GROUND_4,
//	GROUND_5,
//	GROUND_6,
//	GROUND_7,
//	GROUND_8,
//	GROUND_9,
//	GROUND_10,
//	WALL_1,
//	WALL_2,
//	WALL_3,
//	WALL_4,
//	WALL_5,
//	WALL_6,
//	WALL_7,
//	WALL_8,
//	WALL_9,
//	WALL_10,
//	WALL_11,
//	WALL_12,
//	WALL_13,
//	WALL_14,
//	WALL_15,
//	WALL_16, 
//	WALL_17,// this is not a hard thinggy
//	WALL_18,
//	WALL_19,
//	WALL_20,
//	WALL_21,
//	WALL_22,
//	WATER_1, // WOTAH
//	CHEST_1,
//	CHEST_2,
//	NUM_TYPES_TILE
//};

//void load_file(game_map* dst, const char* src);

//void loadSprites(void);
//
//void render_map(game_map* map, AEVec2 offset);
//
//void render_mapFog(game_map* map, AEVec2 offset, AEVec2 pos, int range, int mapoffset);