#pragma once

#include <AEEngine.h>
#include <queue>
#include <vector>
#include <map>
#include <string>

#include "../Map/Map.h"

class PathManager
{
    game_map* Map;
    std::queue<std::pair<AEVec2, AEVec2>> frontier;
    std::map < std::string, AEVec2 > visited;

    std::string VectorToString(AEVec2);

    bool CheckCell(AEVec2, AEVec2, AEVec2);
    bool IsPositionValid(AEVec2);

public:
    
    std::vector<AEVec2> GetPath(AEVec2, AEVec2);

    PathManager(game_map* Map);
    ~PathManager();
};