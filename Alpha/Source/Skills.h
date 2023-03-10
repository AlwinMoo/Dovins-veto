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


enum Bullet_Flag
{
	no_upgrades = 0,
	upgrade1,
	upgrade2
};

void shoot_bullet(GameObject*, GameObject*, f32, f32, int);
void AOE_move(GameObject*, double, double);
void player_blink(GameObject*, f32, f32);