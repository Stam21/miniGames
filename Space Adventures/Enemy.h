/**
 * File:    Enemy.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that that represent the enemy and his interaction with the space game.
 *
 */
#pragma once
#include "games.h"
#include "graphics.h"
#include <vector>


class Enemy : public Games
{
	float direction;
	float speedy;
	float speedx;
	float pos_xx, pos_yy;
	bool shot_initialized = false;
public:

	void draw() override;
	void update() override;
	void init() override;
	void setShotInit(bool s);
	bool getShotInit();
	float getDirection();
	void setDirection(float dir);
	float getPos_X();
	float getPos_Y();
	void setPos_X(float x);
	void setPos_Y(float y);
	Enemy();
	~Enemy();
};
