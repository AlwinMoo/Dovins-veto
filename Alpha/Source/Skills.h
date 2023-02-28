#pragma once
#include "Bevantest.h"
#include "Skills.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
#include <vector>
#include "Map/Map.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <iostream>
#include "Pathfinding/pathfinder.h"
#include <cmath>


#define BULLET_SIZE 10.0f;
#define BULLET_VEL 20.0f;

enum Bullet_Flag
{
	no_upgrades = 0,
	upgrade1,
	upgrade2
};

void shoot_bullet(GameObject*, GameObject*, f32, f32, int);